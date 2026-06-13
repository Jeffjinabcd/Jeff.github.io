import React, { useState, useEffect, useCallback, useRef, useMemo } from 'react';
import {
    View,
    Text,
    TextInput,
    FlatList,
    ActivityIndicator,
    ScrollView,
    Linking,
    Platform,
    Modal,
    Pressable,
    Animated,
    Easing,
    StatusBar,
    SafeAreaView,
    StyleSheet,
    RefreshControl,
    Image,
    useWindowDimensions
} from 'react-native';
import { WebView } from 'react-native-webview';
import { BlurView } from 'expo-blur';
// import * as Haptics from 'expo-haptics';

import { SafeAreaProvider, SafeAreaView as RNSafeAreaView } from 'react-native-safe-area-context';
import AsyncStorage from '@react-native-async-storage/async-storage';
import {
    Calendar,
    Clock,
    Users,
    Star,
    Settings,
    Search,
    ChevronRight,
    Trophy,
    Target,
    Filter,
    MapPin,
    Info,
    AlertCircle,
    BookOpen,
    Menu,
    X,
    Calculator,
    Check,
    Award,
    List,
    TrendingUp,
    RefreshCw,
    ExternalLink,
    Shield,
    Zap,
    Hash,
    Trash2,
    Box,
    Swords,
    ArrowUp,
    ArrowLeft,
    ArrowRight,
    Plus,
    Minus
} from 'lucide-react-native';
import { useFonts, Rajdhani_500Medium, Rajdhani_600SemiBold, Rajdhani_700Bold } from '@expo-google-fonts/rajdhani';
import * as SplashScreen from 'expo-splash-screen';
const STONEHENGE_LOGO = require('./assets/stonehenge.png');
const MAKAPAKA_LOGO = require('./assets/makapaka_logo.png');
const ASSETS = {
    redBlock: require('./assets/red_block.png'),
    blueBlock: require('./assets/blue_block.png'),
    longGoal: require('./assets/long_goal.png'),
    redLongGoal: require('./assets/red_long_goal.png'),
    blueLongGoal: require('./assets/blue_long_goal.png'),
    centerGoal: require('./assets/center_goal.png'),
    redLow: require('./assets/red_low.png'),
    blueLow: require('./assets/blue_low.png'),
    redHigh: require('./assets/red_high.png'),
    blueHigh: require('./assets/blue_high.png'),
    redPark: require('./assets/red_park.png'),
    bluePark: require('./assets/blue_park.png'),
    loader: require('./assets/loader.png'),
    cat1: require('./assets/cat1.jpg'),
    cat2: require('./assets/cat2.jpg'),
    cat3: require('./assets/cat3.jpg'),
    cat4: require('./assets/cat4.jpg'),
    cat5: require('./assets/cat5.jpg'),
    cat6: require('./assets/cat6.jpg'),
};

// Keep the splash screen visible while we fetch resources
SplashScreen.preventAutoHideAsync().catch(console.warn);

import { MatchPredictor } from './src/components/MatchPredictor';
import { Rankings } from './src/components/Rankings';
import { calculateWinProbability } from './src/utils/predictionUtils';
import { calculateStats } from './src/utils/stats';
import { calculateMeowScore, calculateEventScore, getMeowRank, MEOW_RANKS, getEventTier, EVENT_TIERS } from './src/utils/meowScore';
import { FavoritesManager } from './src/utils/FavoritesManager';
import { THEME, THEMES, THEME_IDS } from './src/theme';
import { ThemeProvider, useTheme } from './src/context/ThemeContext';

import { MANUAL_DATA, DEMO_TEAMS, DEMO_EVENTS, DEFAULT_SEASON_ID, PROGRAM_ID, CATEGORIES } from './src/data/manualData';
import { COUNTRY_CODES, COUNTRIES, REGIONS } from './src/data/locations';
import { Header, TabBar, Stepper, ScaleButton, TabFadeView, ScreenTransition, AnimatedListItem, DropdownSelector } from './src/components/UI';
import { DivisionPredictor } from './src/components/DivisionPredictor';
import { LoadingScreen } from './src/components/LoadingScreen';
import { ROBOT_EVENTS_API_KEY } from './src/config';



const CustomSplashScreen = ({ onFinish }) => {
    const [visible, setVisible] = useState(true);
    const opacity = useRef(new Animated.Value(1)).current;

    useEffect(() => {
        // Keep visible for a moment, then fade out
        const timer = setTimeout(() => {
            Animated.timing(opacity, {
                toValue: 0,
                duration: 500,
                useNativeDriver: true,
            }).start(() => {
                setVisible(false);
                onFinish && onFinish();
            });
        }, 1500); // Display for 1.5 seconds

        return () => clearTimeout(timer);
    }, []);

    if (!visible) return null;

    return (
        <Animated.View style={[StyleSheet.absoluteFill, {
            backgroundColor: '#000',
            justifyContent: 'center',
            alignItems: 'center',
            opacity,
            zIndex: 9999
        }]}>
            <View style={{ flexDirection: 'column', alignItems: 'center', gap: 20 }}>
                <Image source={MAKAPAKA_LOGO} style={{ width: 300, height: 300, resizeMode: 'contain' }} />
                <Image source={STONEHENGE_LOGO} style={{ width: 300, height: 300, resizeMode: 'contain' }} />
            </View>
        </Animated.View>
    );
};

// --- API Service ---
const useRobotEvents = (apiKey, proxyUrl) => {
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState(null);

    const request = useCallback(async (endpoint, params = {}, signal = null) => {
        if (!apiKey) return null;
        setLoading(true);
        setError(null);

        try {
            let url = endpoint.startsWith('http') ? endpoint : `https://www.robotevents.com/api/v2${endpoint}`;
            const queryParams = [];

            Object.keys(params).forEach(key => {
                if (params[key] !== undefined && params[key] !== null) {
                    if (Array.isArray(params[key])) {
                        params[key].forEach(value => {
                            queryParams.push(`${encodeURIComponent(key)}[]=${encodeURIComponent(value)}`);
                        });
                    } else {
                        queryParams.push(`${encodeURIComponent(key)}=${encodeURIComponent(params[key])}`);
                    }
                }
            });

            if (queryParams.length > 0) {
                url += `?${queryParams.join('&')}`;
            }



            const response = await fetch(url, {
                headers: {
                    'Authorization': `Bearer ${apiKey}`,
                    'Accept': 'application/json',
                },
                signal: signal // Pass the abort signal
            });

            if (!response.ok) {
                let errorMessage;
                try {
                    const errorData = await response.json();
                    errorMessage = `API Error: ${response.status} ${JSON.stringify(errorData)}`;
                } catch (e) {
                    errorMessage = `API Error: ${response.status} ${response.statusText}`;
                }
                const error = new Error(errorMessage);
                error.statusCode = response.status; // Attach status code
                throw error;
            }
            const data = await response.json();
            setLoading(false);
            return data;
        } catch (err) {
            if (err.name === 'AbortError') {
                console.log('Fetch aborted');
            } else {
                console.error("API Request Failed:", err);
                setError(err.message);
            }
            setLoading(false);
            return null;
        }
    }, [apiKey]);

    return { request, loading, error };
};

const CategoryFilter = ({ selected, onSelect }) => {
    const { theme: THEME } = useTheme();
    return (
        <View style={{ flexDirection: 'row', padding: 12, gap: 8, backgroundColor: THEME.colors.surface, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
            <ScrollView horizontal showsHorizontalScrollIndicator={false} contentContainerStyle={{ gap: 8 }}>
                {CATEGORIES.map(cat => (
                    <ScaleButton
                        key={cat.id}
                        onPress={() => onSelect(cat)}
                        style={{
                            paddingHorizontal: 16,
                            paddingVertical: 8,
                            borderRadius: 20,
                            backgroundColor: selected.id === cat.id ? THEME.colors.primary : THEME.colors.background,
                            borderWidth: 1,
                            borderColor: selected.id === cat.id ? THEME.colors.primary : THEME.colors.border
                        }}
                    >
                        <Text style={{
                            color: selected.id === cat.id ? THEME.colors.background : THEME.colors.text.secondary,
                            fontWeight: '600',
                            fontSize: 14
                        }}>
                            {cat.label}
                        </Text>
                    </ScaleButton>
                ))}
            </ScrollView>
        </View>
    );
};

// --- Views ---

// Helper for Landscape Score
const LandscapeScoreView = ({ redScore, blueScore, theme }) => {
    return (
        <View style={{ position: 'absolute', top: 0, left: 0, right: 0, bottom: 0, backgroundColor: theme.colors.background, justifyContent: 'center', alignItems: 'center', zIndex: 9999 }}>
            <StatusBar hidden={true} />
            <View style={{ flexDirection: 'row', alignItems: 'center', width: '100%', paddingHorizontal: 20, marginTop: -30 }}>
                <Text style={{ flex: 1, fontSize: 180, fontWeight: '900', color: theme.colors.error, includeFontPadding: false, textAlign: 'right' }}>
                    {redScore.toString().padStart(3, '0')}
                </Text>
                <Text style={{ width: 80, fontSize: 120, fontWeight: '900', color: theme.colors.text.secondary, includeFontPadding: false, textAlign: 'center' }}>
                    -
                </Text>
                <Text style={{ flex: 1, fontSize: 180, fontWeight: '900', color: theme.colors.blueAlliance, includeFontPadding: false, textAlign: 'left' }}>
                    {blueScore.toString().padStart(3, '0')}
                </Text>
            </View>
        </View>
    );
};

const ScoreCalculator = React.memo(({
    mode, setMode,
    goals, setGoals,
    matchData, setMatchData,
    skillsData, setSkillsData,
    skillsGoals, setSkillsGoals,
    calculateMatchScore, calculateSkillsScore
}) => {
    const { theme: THEME } = useTheme();
    const { width, height } = useWindowDimensions();
    const isLandscape = width > height;








    const updateMatch = useCallback((color, field, value) => {
        setMatchData(prev => {
            const newData = { ...prev };
            // Check limits for center goal blocks
            if (field === 'centerUpper' || field === 'centerLower') {
                const newValue = Math.max(0, value);

                // Max 44 total check
                // Current blocks on goals
                const currentLongGoalBlocks = goals.reduce((sum, g) => sum + (color === 'red' ? g.redBlocks : g.blueBlocks), 0);

                // Blocks in THIS center goal field are 'newValue'
                // Blocks in the OTHER center goal field need to be fetched from prev
                const otherCenterField = field === 'centerUpper' ? 'centerLower' : 'centerUpper';
                const currentCenterBlocks = newValue + prev[color][otherCenterField];

                if (currentLongGoalBlocks + currentCenterBlocks > 44) return prev;
            }

            if (field === 'parking' && value > 2) return prev;
            newData[color] = { ...newData[color], [field]: value };
            return newData;
        });
    }, [goals]);

    const updateSkills = (field, value) => {
        setSkillsData(prev => ({
            ...prev,
            [field]: value
        }));
    };

    const resetCenterGoals = () => {
        setMatchData(prev => ({
            ...prev,
            red: { ...prev.red, centerUpper: 0, centerLower: 0 },
            blue: { ...prev.blue, centerUpper: 0, centerLower: 0 }
        }));
    };

    const updateGoal = useCallback((id, field, value) => {
        setGoals(prev => {
            // Calculate current totals for validation
            if (field === 'redBlocks' || field === 'blueBlocks') {
                const newValue = Math.max(0, value);
                const targetGoal = prev.find(g => g.id === id);

                // 1. Check Goal Capacity (Max 15 total blocks per goal)
                const currentOther = field === 'redBlocks' ? targetGoal.blueBlocks : targetGoal.redBlocks;
                if (newValue + currentOther > 15) return prev;

                // 2. Check Alliance Total Limit (Max 44 blocks total for alliance)
                const allianceField = field;
                const allianceColor = field === 'redBlocks' ? 'red' : 'blue';

                // Sum blocks on OTHER goals for this alliance
                const currentLongGoalOther = prev.reduce((sum, g) => sum + (g.id === id ? 0 : g[allianceField]), 0);

                // Sum blocks on Center Goal for this alliance
                const centerGoalBlocks = (matchData[allianceColor].centerUpper || 0) + (matchData[allianceColor].centerLower || 0);

                if (currentLongGoalOther + newValue + centerGoalBlocks > 44) return prev;

                return prev.map(g => g.id === id ? { ...g, [field]: newValue } : g);
            }

            if (field === 'owner') {
                return prev.map(g => g.id === id ? { ...g, owner: value } : g);
            }

            if (field === 'reset') {
                return prev.map(g => g.id === id ? { ...g, redBlocks: 0, blueBlocks: 0, owner: null } : g);
            }
            return prev;
        });
    }, [matchData]);
    const reset = () => {
        if (mode === 'match') {
            setMatchData({
                red: { autoBonus: false, blocks: 0, longGoals: 0, centerUpper: 0, centerLower: 0, parking: 0 },
                blue: { autoBonus: false, blocks: 0, longGoals: 0, centerUpper: 0, centerLower: 0, parking: 0 }
            });
            setGoals([
                { id: 1, redBlocks: 0, blueBlocks: 0, owner: null },
                { id: 2, redBlocks: 0, blueBlocks: 0, owner: null }
            ]);
        } else {
            setSkillsData({ blocks: 0, longGoals: 0, centerUpper: 0, centerLower: 0, parking: 0, clearedLoaders: 0, clearedParking: 0 });
            setSkillsGoals([
                { id: 101, redBlocks: 0, blueBlocks: 0, owner: null },
                { id: 102, redBlocks: 0, blueBlocks: 0, owner: null }
            ]);
        }
    };

    const updateSkillsGoal = (id, field, value) => {
        setSkillsGoals(prev => {
            if (field === 'redBlocks' && value >= 0 && value <= 15) {
                return prev.map(g => g.id === id ? { ...g, redBlocks: value } : g);
            }
            if (field === 'owner') {
                const newVal = (value === 'red' && prev.find(g => g.id === id).owner === 'red') ? null : value;
                // Toggle off if same, or set new. Skills only really uses 'red' (owned) or null.
                return prev.map(g => g.id === id ? { ...g, owner: value } : g);
            }
            if (field === 'reset') {
                return prev.map(g => g.id === id ? { ...g, redBlocks: 0, blueBlocks: 0, owner: null } : g);
            }
            return prev;
        });
    };



    const VisualCard = ({ label, image, value, onChange, max, color, isFilled, transparent, small, controlSide = 'left', flipImage = false, imageScale = 1 }) => {
        const StepperControls = () => (
            <View style={{ alignItems: 'center', gap: 0 }}>
                <ScaleButton
                    onPress={() => onChange(Math.min(max, value + 1))}
                    hitSlop={{ top: 40, bottom: 40, left: 40, right: 40 }}
                    style={{
                        width: 28, height: 28, borderRadius: 8,
                        backgroundColor: color, borderColor: color, borderWidth: 1,
                        justifyContent: 'center', alignItems: 'center',
                        opacity: value >= max ? 0.3 : 1
                    }}
                >
                    <Plus size={16} color="white" />
                </ScaleButton>

                <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>
                    {value}
                </Text>

                <ScaleButton
                    onPress={() => onChange(Math.max(0, value - 1))}
                    hitSlop={{ top: 15, bottom: 15, left: 40, right: 40 }}
                    style={{
                        width: 28, height: 28, borderRadius: 8,
                        backgroundColor: color + '20', borderWidth: 2, borderColor: color,
                        justifyContent: 'center', alignItems: 'center'
                    }}
                >
                    <Minus size={16} color={THEME.colors.text.primary} />
                </ScaleButton>
            </View>
        );

        return (
            <View style={{
                width: small ? 140 : '48%', backgroundColor: transparent ? 'transparent' : THEME.colors.surface, borderRadius: 16,
                padding: small ? 4 : 12, marginBottom: 8, alignItems: 'center', borderWidth: transparent ? 0 : 1, borderColor: transparent ? 'transparent' : THEME.colors.border,
                shadowColor: transparent ? 'transparent' : '#000', shadowOffset: { width: 0, height: 2 }, shadowOpacity: 0.1, shadowRadius: 4, elevation: transparent ? 0 : 2
            }}>
                {label && (
                    <Text style={{ fontSize: small ? 10 : 14, fontWeight: 'bold', color: THEME.colors.text.secondary, marginBottom: 4, textAlign: 'center' }}>
                        {label}
                    </Text>
                )}
                <View style={{ flexDirection: 'row', alignItems: 'flex-end', justifyContent: 'center', gap: 12 }}>
                    {controlSide === 'left' && <StepperControls />}

                    <View style={{ width: small ? 85 : 80, height: small ? 85 : 80, marginBottom: small ? 4 : 12, justifyContent: 'center', alignItems: 'center', position: 'relative' }}>
                        <Image source={image} style={{ width: '100%', height: '100%', resizeMode: 'contain', opacity: value > 0 ? 1 : 0.5, transform: [{ scaleX: flipImage ? -1 : 1 }, { scale: imageScale }] }} />
                        {isFilled && value > 0 && (
                            <View style={{
                                position: 'absolute', bottom: 0, left: 0, right: 0,
                                height: `${(value / (max || 1)) * 100}%`,
                                backgroundColor: color, opacity: 0.3, borderRadius: 8
                            }} />
                        )}
                    </View>

                    {controlSide === 'right' && <StepperControls />}
                </View>
            </View>
        );
    };

    const renderMatchSection = (alliance) => {
        // ... (We still need this for Auto Bonus and Score Display, but not for rings/goals anymore)
        const data = matchData[alliance];
        const color = alliance === 'red' ? THEME.colors.error : THEME.colors.blueAlliance;

        return (
            <View style={{ marginBottom: 24 }}>
                {/* Header with Score */}
                <View style={{ flexDirection: 'row', alignItems: 'center', gap: 8, marginBottom: 16, paddingHorizontal: 4 }}>
                    <View style={{ width: 12, height: 12, borderRadius: 6, backgroundColor: color }} />
                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: THEME.colors.text.primary, textTransform: 'capitalize' }}>
                        {alliance} Alliance
                    </Text>
                    <View style={{ flex: 1 }} />
                    <Text style={{ fontSize: 18, fontWeight: 'bold', color }}>{calculateMatchScore(alliance, data)} Pt</Text>
                </View>

                {/* Auto Bonus & Other Cards */}
                <View style={{ flexDirection: 'row', flexWrap: 'wrap', justifyContent: 'space-between' }}>
                    {/* Auto Bonus Button */}
                    <ScaleButton
                        onPress={() => updateMatch(alliance, 'autoBonus', !data.autoBonus)}
                        hitSlop={{ top: 40, bottom: 40, left: 40, right: 40 }}
                        style={{
                            width: '100%', height: 60, backgroundColor: THEME.colors.surface, borderRadius: 12,
                            padding: 12, marginBottom: 16, flexDirection: 'row', alignItems: 'center', justifyContent: 'center', gap: 12,
                            borderWidth: 2, borderColor: data.autoBonus ? color : THEME.colors.border
                        }}
                    >
                        <Award size={28} color={data.autoBonus ? color : THEME.colors.text.secondary} />
                        <Text style={{ fontSize: 18, fontWeight: 'bold', color: data.autoBonus ? color : THEME.colors.text.secondary }}>Auto Bonus (+10)</Text>
                    </ScaleButton>


                    <VisualCard label="Parking" image={alliance === 'red' ? ASSETS.redPark : ASSETS.bluePark} value={data.parking} onChange={v => updateMatch(alliance, 'parking', v)} max={2} color={color} isFilled />
                </View>
            </View>
        );
    };

    if (isLandscape) {
        return <LandscapeScoreView redScore={calculateMatchScore('red', matchData.red)} blueScore={calculateMatchScore('blue', matchData.blue)} theme={THEME} />;
    }

    return (
        <View style={{ flex: 1, backgroundColor: THEME.colors.background }}>
            {/* Header - Compact */}
            <ScrollView contentContainerStyle={{ padding: 16, paddingBottom: 150, paddingTop: 10 }}>


                {/* Autonomous Bonus Selector (Match Mode Only) */}
                {mode === 'match' && (
                    <View style={{ marginBottom: 0, height: 44, justifyContent: 'center', marginTop: 20, zIndex: 100 }} pointerEvents="box-none">
                        {/* Center: Auto Bonus Buttons (Absolutely Positioned for True Center) */}
                        <View style={{ position: 'absolute', left: 0, right: 0, flexDirection: 'row', justifyContent: 'center', gap: 16, top: 0, zIndex: 50 }} pointerEvents="box-none">
                            <ScaleButton
                                onPress={() => updateMatch('red', 'autoBonus', !matchData.red.autoBonus)}
                                hitSlop={{ top: 40, bottom: 40, left: 40, right: 40 }}
                            >
                                <View
                                    pointerEvents="none"
                                    style={{
                                        width: 60, height: 42, borderRadius: 10,
                                        backgroundColor: matchData.red.autoBonus ? THEME.colors.error : 'transparent',
                                        borderWidth: 2, borderColor: THEME.colors.error,
                                        justifyContent: 'center', alignItems: 'center'
                                    }}
                                >
                                    <Text style={{ fontSize: 22, lineHeight: 26, fontWeight: 'bold', color: matchData.red.autoBonus ? 'white' : THEME.colors.error, includeFontPadding: false, textAlignVertical: 'center' }}>A</Text>
                                </View>
                            </ScaleButton>

                            <ScaleButton
                                onPress={() => updateMatch('blue', 'autoBonus', !matchData.blue.autoBonus)}
                                hitSlop={{ top: 40, bottom: 40, left: 40, right: 40 }}
                            >
                                <View
                                    pointerEvents="none"
                                    style={{
                                        width: 60, height: 42, borderRadius: 10,
                                        backgroundColor: matchData.blue.autoBonus ? THEME.colors.blueAlliance : 'transparent',
                                        borderWidth: 2, borderColor: THEME.colors.blueAlliance,
                                        justifyContent: 'center', alignItems: 'center'
                                    }}
                                >
                                    <Text style={{ fontSize: 22, lineHeight: 26, fontWeight: 'bold', color: matchData.blue.autoBonus ? 'white' : THEME.colors.blueAlliance, includeFontPadding: false, textAlignVertical: 'center' }}>A</Text>
                                </View>
                            </ScaleButton>
                        </View>

                        {/* Left & Right Controls */}
                        <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', width: '100%', zIndex: 1 }} pointerEvents="box-none">
                            {/* Left: Mode Switcher */}
                            <View style={{ flexDirection: 'column', backgroundColor: THEME.colors.background, borderRadius: 8, padding: 2, height: 'auto', gap: 2 }}>
                                {['match', 'skills'].map(m => (
                                    <ScaleButton
                                        key={m}
                                        onPress={() => setMode(m)}
                                        style={{
                                            width: 50, height: 28, borderRadius: 6,
                                            backgroundColor: mode === m ? THEME.colors.primary : 'transparent',
                                            justifyContent: 'center', alignItems: 'center'
                                        }}
                                    >
                                        <Text style={{ fontWeight: 'bold', fontSize: 10, color: mode === m ? 'black' : THEME.colors.text.secondary }}>
                                            {m.charAt(0).toUpperCase() + m.slice(1)}
                                        </Text>
                                    </ScaleButton>
                                ))}
                            </View>

                            {/* Right: Reset Button */}
                            <ScaleButton onPress={reset} style={{ width: 42, height: 42, justifyContent: 'center', alignItems: 'center', backgroundColor: THEME.colors.background, borderRadius: 8 }}>
                                <Trash2 size={20} color={THEME.colors.text.secondary} />
                            </ScaleButton>
                        </View>
                    </View>
                )}

                {mode === 'match' ? (
                    <View>
                        {/* Interactive Goal Widgets (Shared) */}
                        {goals.map(g => <View key={g.id} style={{ marginBottom: 10, marginTop: 0, zIndex: 50 }}><GoalWidget goal={g} updateGoal={updateGoal} /></View>)}
                        <View style={{ marginTop: 0, zIndex: 10 }}>
                            <CenterGoalWidget matchData={matchData} updateMatch={updateMatch} resetCenterGoals={resetCenterGoals} />
                        </View>

                        <View
                            pointerEvents="box-none"
                            style={{ flexDirection: 'row', justifyContent: 'center', gap: 10, marginVertical: 0, marginTop: -15 }}
                        >
                            <VisualCard label={null} image={ASSETS.redPark} value={matchData.red.parking} onChange={v => updateMatch('red', 'parking', v)} max={2} color={THEME.colors.error} isFilled transparent small controlSide="left" />
                            <VisualCard label={null} image={ASSETS.bluePark} value={matchData.blue.parking} onChange={v => updateMatch('blue', 'parking', v)} max={2} color={THEME.colors.blueAlliance} isFilled transparent small controlSide="right" flipImage={true} imageScale={1.2} />
                        </View>
                    </View>
                ) : (
                    <View>
                        {/* Interactive Widgets for Skills */}
                        {skillsGoals.map(g => (
                            <GoalWidget
                                key={g.id}
                                goal={g}
                                updateGoal={updateSkillsGoal}
                                isSkills={true}
                            />
                        ))}

                        <CenterGoalWidget
                            matchData={{ red: skillsData, blue: {} }}
                            updateMatch={(a, t, v) => updateSkills(t, v)}
                            resetCenterGoals={() => {
                                updateSkills('centerUpper', 0);
                                updateSkills('centerLower', 0);
                                updateSkills('centerUpperOwned', null);
                                updateSkills('centerLowerOwned', null);
                            }}
                            isSkills={true}
                        />

                        <View style={{ flexDirection: 'row', flexWrap: 'wrap', justifyContent: 'space-between', marginTop: 16 }}>
                            <VisualCard label="Empty Loaders" image={ASSETS.loader} value={skillsData.clearedLoaders} onChange={v => updateSkills('clearedLoaders', v)} max={4} color="#A855F7" />
                            <VisualCard label="Park Clear" image={ASSETS.redPark} value={skillsData.clearedParking} onChange={v => updateSkills('clearedParking', v)} max={2} color="#A855F7" />
                            <VisualCard label={null} image={ASSETS.redPark} value={skillsData.parking} onChange={v => updateSkills('parking', v)} max={1} color="#A855F7" />
                        </View>
                        <View style={{ marginBottom: 16 }} />
                    </View>
                )}
            </ScrollView>

            {/* Sticky Score Footer */}
            <View style={{
                position: 'absolute', bottom: 85, left: 20, right: 20,
                backgroundColor: THEME.colors.surface,
                borderRadius: 20, borderWidth: 1, borderColor: THEME.colors.border,
                paddingVertical: 12, paddingHorizontal: 16,
                flexDirection: 'row', justifyContent: 'center', alignItems: 'center',
                shadowColor: '#000', shadowOffset: { width: 0, height: 4 }, shadowOpacity: 0.2, shadowRadius: 8, elevation: 10
            }}>
                {mode === 'match' ? (
                    <View style={{ flexDirection: 'row', alignItems: 'center', gap: 16 }}>
                        <Text style={{ fontSize: 32, fontWeight: 'bold', color: THEME.colors.error, minWidth: 50, textAlign: 'center' }}>{calculateMatchScore('red', matchData.red)}</Text>
                        <Text style={{ fontSize: 20, fontWeight: 'bold', color: THEME.colors.text.secondary }}>-</Text>
                        <Text style={{ fontSize: 32, fontWeight: 'bold', color: THEME.colors.blueAlliance, minWidth: 50, textAlign: 'center' }}>{calculateMatchScore('blue', matchData.blue)}</Text>
                    </View>
                ) : (
                    <Text style={{ fontSize: 40, fontWeight: 'bold', color: '#A855F7' }}>{calculateSkillsScore(skillsData)}</Text>
                )}
            </View>
        </View>
    );
});

const PDF_SECTIONS = [
    { id: 'intro', label: 'Intro', page: 1 },
    { id: 'def', label: 'Definitions', page: 6 },
    { id: 'scoring', label: 'Scoring', page: 12 },
    { id: 'safety', label: 'Safety <S>', page: 15 },
    { id: 'general', label: 'General <G>', page: 16 },
    { id: 'specific', label: 'Game <SG>', page: 22 },
    { id: 'robot', label: 'Robot <R>', page: 29 },
    { id: 'tourney', label: 'Tournament <T>', page: 38 },
    { id: 'app_a', label: 'App A: Field', page: 44 },
    { id: 'app_b', label: 'App B: Skills', page: 46 },
    { id: 'app_c', label: 'App C: Coding', page: 50 },
];

const ManualView = React.memo(() => {
    const [key, setKey] = useState(0);
    const currentUrl = 'https://content.vexrobotics.com/docs/25-26/v5rc-push-back/docs/Push-Back-2.2.pdf';
    const webViewRef = useRef(null);

    const scrollToTop = () => {
        if (webViewRef.current) {
            // Try JS scroll first
            webViewRef.current.injectJavaScript(`
                window.scrollTo(0, 0); 
                true;
            `);
            // Also force reload to top if needed using key, but JS is preferred for smoothness
            // setKey(prev => prev + 1); 
        }
    };

    return (
        <RNSafeAreaView edges={['top']} style={{ flex: 1, backgroundColor: '#f5f5f5' }}>
            <View style={{ flex: 1, position: 'relative' }}>
                <WebView
                    ref={webViewRef}
                    key={key}
                    source={{ uri: currentUrl }}
                    style={{ flex: 1, backgroundColor: 'white' }}
                    startInLoadingState={true}
                    renderLoading={() => <LoadingScreen message="Loading Manual..." />}
                    javaScriptEnabled={true}
                    domStorageEnabled={true}
                />

                {/* Floating Scroll to Top Button (Apple Liquid Glass Style) */}
                <ScaleButton
                    onPress={scrollToTop}
                    style={{
                        position: 'absolute', bottom: 130, right: 30, zIndex: 999,
                        width: 50, height: 50, borderRadius: 25,
                        overflow: 'hidden', // Clip the BlurView
                        shadowColor: '#000', shadowOffset: { width: 0, height: 4 }, shadowOpacity: 0.2, shadowRadius: 8, // Softer, deeper shadow
                        elevation: 10
                    }}
                >
                    <BlurView
                        intensity={40} // Subtle glass effect
                        tint="systemThinMaterialDark" // or 'dark' / 'light' depending on preference. ThinMaterialDark looks very modern/premium.
                        style={{
                            flex: 1,
                            alignItems: 'center', justifyContent: 'center',
                            backgroundColor: 'rgba(50, 50, 50, 0.3)' // Subtle tint overlay
                        }}
                    >
                        <ArrowUp size={24} color="white" />
                    </BlurView>
                </ScaleButton>
            </View>
        </RNSafeAreaView>
    );
});

// Global cache for events to persist across tab switches
const eventsCache = {};

const RenderEventItem = React.memo(({ item, index, onSelectEvent, styles, THEME }) => (
    <AnimatedListItem index={index} delay={30}>
        <ScaleButton onPress={() => onSelectEvent(item)} style={styles.card}>
            <View style={{ flex: 1, paddingRight: 12 }}>
                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 4 }}>
                    <Text style={[styles.cardSubtitle, { marginBottom: 0 }]}>{item.season?.name || 'V5RC'}</Text>
                    <View style={{
                        marginLeft: 8, paddingHorizontal: 6, paddingVertical: 2, borderRadius: 4,
                        backgroundColor: getEventTier(item).color
                    }}>
                        <Text style={{ fontSize: 10, fontWeight: 'bold', color: 'white' }}>
                            {getEventTier(item).name}
                        </Text>
                    </View>
                </View>
                <Text style={styles.cardTitle} numberOfLines={2}>{item.name}</Text>
                <View style={styles.cardFooter}>
                    <View style={styles.cardFooterItem}><Calendar size={12} color={THEME.colors.text.secondary} /><Text style={styles.cardFooterText}>{new Date(item.start).toLocaleDateString()}</Text></View>
                    <View style={styles.cardFooterItem}><MapPin size={12} color={THEME.colors.text.secondary} /><Text style={styles.cardFooterText}>{item.location?.city}, {item.location?.region}</Text></View>
                </View>
            </View>
            <ChevronRight size={20} color="#d1d5db" style={{ marginRight: 4 }} />
        </ScaleButton>
    </AnimatedListItem>
));

const EventList = React.memo(({ onSelectEvent, apiKey, apiRequest, loading: apiLoading, selectedSeason, category, setCategory }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const [allEvents, setAllEvents] = useState([]);
    const [filteredEvents, setFilteredEvents] = useState([]);
    const [search, setSearch] = useState('');
    const [debouncedSearch, setDebouncedSearch] = useState(''); // Debounced search term
    const [isLoading, setIsLoading] = useState(false);
    const [progress, setProgress] = useState(0);

    // Pull to refresh
    const [refreshing, setRefreshing] = useState(false);

    // Debounce search input
    useEffect(() => {
        const timer = setTimeout(() => {
            setDebouncedSearch(search);
        }, 300); // 300ms debounce
        return () => clearTimeout(timer);
    }, [search]);

    const onRefresh = useCallback(() => {
        setRefreshing(true);
        // Force re-fetch by clearing cache for this key
        const cacheKey = `${selectedSeason.id}-${category.programId}`;
        delete eventsCache[cacheKey];

        // Re-trigger the effect
        setAllEvents([]);
        // The useEffect will run because allEvents changed (or we can extract the load function)
        // Actually, better to extract the load function.
        loadAllEvents(true);
    }, [selectedSeason, category]);

    const loadAllEvents = useCallback(async (isRefresh = false) => {
        if (!apiKey || !selectedSeason) {
            setRefreshing(false);
            return;
        }

        const cacheKey = `${selectedSeason.id}-${category.programId}`;

        if (!isRefresh && eventsCache[cacheKey]) {
            setAllEvents(eventsCache[cacheKey]);
            setFilteredEvents(eventsCache[cacheKey]);
            return;
        }

        setIsLoading(true);
        setProgress(0);
        let accumulated = [];
        let lastPage = 1;
        const PER_PAGE = 250;

        try {
            // Fetch first page to get metadata
            const params = {
                program: [category.programId],
                season: [selectedSeason.id],
                page: 1,
                per_page: PER_PAGE
            };

            if (category.grade && category.grade !== 'All') {
                params.grade_level = [category.grade];
            }

            const firstPageData = await apiRequest('/events', params);

            if (firstPageData && firstPageData.data) {
                accumulated = [...firstPageData.data];
                lastPage = firstPageData.meta.last_page;

                // Fetch remaining pages
                for (let p = 2; p <= lastPage; p++) {
                    setProgress(p / lastPage);
                    // Small delay to be polite and avoid 429
                    await new Promise(r => setTimeout(r, 180));

                    const nextParams = { ...params, page: p };
                    const nextData = await apiRequest('/events', nextParams);
                    if (nextData && nextData.data) {
                        accumulated = [...accumulated, ...nextData.data];
                    }
                }
            }

            // Deduplicate just in case
            const uniqueEvents = Array.from(new Map(accumulated.map(item => [item.id, item])).values());

            eventsCache[cacheKey] = uniqueEvents;
            setAllEvents(uniqueEvents);
            setFilteredEvents(uniqueEvents);
        } catch (e) {
            console.error("Failed to load events:", e);
        } finally {
            setIsLoading(false);
            setRefreshing(false);
            setProgress(1);
        }
    }, [apiKey, selectedSeason, category, apiRequest]);

    // Load events for the selected season/program
    useEffect(() => {
        // Clear events immediately when category changes
        setAllEvents([]);
        setFilteredEvents([]);

        loadAllEvents();
    }, [loadAllEvents, category]);

    const [country, setCountry] = useState('');
    const [region, setRegion] = useState('');

    // Filter events when debounced search or filters change
    useEffect(() => {
        let filtered = allEvents;

        // 1. Text Search (using debounced value)
        if (debouncedSearch.trim() !== '') {
            const lowerSearch = debouncedSearch.toLowerCase();
            filtered = filtered.filter(item =>
                (item.name || '').toLowerCase().includes(lowerSearch) ||
                (item.sku || '').toLowerCase().includes(lowerSearch) ||
                (item.location && (
                    (item.location.city || '').toLowerCase().includes(lowerSearch) ||
                    (item.location.region || '').toLowerCase().includes(lowerSearch)
                ))
            );
        }

        // 2. Country Filter
        if (country.trim()) {
            const countryTrim = country.trim();
            const countryCode = COUNTRY_CODES[countryTrim];
            filtered = filtered.filter(item => {
                if (!item.location) return false;
                const c = item.location.country;
                // Match Name or Code
                return c === countryTrim || c === countryCode;
            });
        }

        // 3. Region Filter
        if (region.trim()) {
            filtered = filtered.filter(item => item.location && item.location.region === region.trim());
        }

        setFilteredEvents(filtered);
    }, [debouncedSearch, allEvents, country, region]);

    return (
        <View style={styles.viewContainer}>
            {isLoading && <LoadingScreen progress={progress} message="Loading Events..." />}
            <CategoryFilter selected={category} onSelect={setCategory} />
            <View style={styles.searchHeader}>
                <View style={styles.searchBar}>
                    <Search size={18} color={THEME.colors.text.secondary} />
                    <TextInput
                        style={styles.searchInput}
                        placeholder={apiKey ? "Search Events (Name, SKU, City)..." : "Demo Mode"}
                        placeholderTextColor={THEME.colors.text.secondary}
                        value={search}
                        onChangeText={setSearch} // Updates local state immediately, effect handles debounce
                    />
                </View>
            </View>

            {/* Filters: Country & Region */}
            <View style={{ flexDirection: 'row', gap: 8, paddingHorizontal: 16, paddingBottom: 8, marginTop: 8, zIndex: 10 }}>
                <View style={{ flex: 1 }}>
                    <DropdownSelector
                        label="Country"
                        placeholder="All Countries"
                        value={country}
                        options={[{ label: 'Clear Filter', value: '' }, ...COUNTRIES]}
                        onSelect={(val) => {
                            setCountry(val);
                            setRegion(''); // Reset region when country changes
                        }}
                    />
                </View>
                <View style={{ flex: 1 }}>
                    <DropdownSelector
                        label="Region"
                        placeholder="All Regions"
                        value={region}
                        options={[{ label: 'Clear Filter', value: '' }, ...(country && REGIONS[country] ? REGIONS[country] : [])]}
                        onSelect={setRegion}
                        hasSearch={true}
                    />
                </View>
            </View>

            <FlatList
                data={filteredEvents}
                keyExtractor={item => item.id.toString()}
                contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                initialNumToRender={10}
                maxToRenderPerBatch={10}
                windowSize={5}
                refreshControl={
                    <RefreshControl refreshing={refreshing} onRefresh={onRefresh} tintColor={THEME.colors.primary} />
                }
                renderItem={({ item, index }) => (
                    <RenderEventItem
                        item={item}
                        index={index}
                        onSelectEvent={onSelectEvent}
                        styles={styles}
                        THEME={THEME}
                    />
                )}
            />
        </View>
    );
});

const RenderTeamItem = React.memo(({ item, index, onSelectTeam, styles, THEME }) => (
    <AnimatedListItem index={index} delay={30}>
        <ScaleButton onPress={() => onSelectTeam(item)} style={styles.listItem}>
            <View style={styles.listItemContent}>
                <View style={{ flex: 1 }}>
                    <Text style={styles.listItemTitle} numberOfLines={1}>
                        <Text style={{ fontWeight: 'bold', color: THEME.colors.primary }}>{item.number}</Text>
                        <Text>  {item.team_name}</Text>
                    </Text>
                    <Text style={styles.listItemSubtitle}>
                        {[item.location?.city, item.location?.region, item.location?.country].filter(Boolean).join(', ')}
                        {item.organization ? ` �?${item.organization}` : ''}
                    </Text>
                </View>
            </View>
            <ChevronRight size={20} color="#d1d5db" />
        </ScaleButton>
    </AnimatedListItem>
));

const TeamList = React.memo(({ onSelectTeam, apiKey, apiRequest, loading, selectedSeason, category, setCategory }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const [teams, setTeams] = useState(apiKey ? [] : DEMO_TEAMS);
    const [search, setSearch] = useState('');
    const [country, setCountry] = useState('');
    const [region, setRegion] = useState('');
    const [page, setPage] = useState(1);
    const [hasMore, setHasMore] = useState(true);
    const [isFetchingMore, setIsFetchingMore] = useState(false);
    const [isLoading, setIsLoading] = useState(false);
    const [progress, setProgress] = useState(0);
    const abortControllerRef = useRef(null);

    const fetchTeams = useCallback(async (pageToLoad, shouldReset = false) => {
        if (!apiKey) return;

        let currentController;

        if (shouldReset) {
            if (abortControllerRef.current) {
                abortControllerRef.current.abort();
            }
            currentController = new AbortController();
            abortControllerRef.current = currentController;
        } else {
            currentController = abortControllerRef.current;
            if (!currentController) {
                currentController = new AbortController();
                abortControllerRef.current = currentController;
            }
        }

        setIsFetchingMore(true);
        if (pageToLoad === 1) {
            setIsLoading(true);
            setProgress(0.2); // Start progress
        }

        const params = {
            program: [category.programId],
            page: pageToLoad,
            per_page: 20
        };

        if (category.grade && category.grade !== 'All') {
            params.grade_level = [category.grade];
        }

        if (selectedSeason && selectedSeason.program && selectedSeason.program.id === category.programId) {
            params.season = [selectedSeason.id];
        }

        // Add Country/Region Filters
        if (country) {
            const code = COUNTRY_CODES[country];
            if (code) params['country[]'] = code;
        }
        if (region) {
            params.region = [region];
        }

        // "Deep Expansion" only if no region/country filters? 
        // Or combine them? If country/region is set, searching by number + variations might return nothing if location mismatches.
        // It helps to narrow down.
        if (search.length > 0) {
            // "Deep Expansion" Strategy
            const variations = [search];
            const suffixes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.split('');
            const digits = '0123456789'.split('');

            // 2. Suffixes
            suffixes.forEach(s => variations.push(search + s));

            // 3. Next Digit
            digits.forEach(d => {
                const withDigit = search + d;
                variations.push(withDigit);
                // 4. Next Digit + Suffix
                suffixes.forEach(s => variations.push(withDigit + s));
            });

            params.number = variations;
            delete params.q;
            delete params.per_page;
        }



        try {
            if (pageToLoad === 1) setProgress(0.5); // Mid-way
            const data = await apiRequest('/teams', params, currentController.signal);

            if (!currentController.signal.aborted) {
                if (data && data.data) {
                    if (shouldReset) {
                        setTeams(data.data);
                    } else {
                        setTeams(prev => [...prev, ...data.data]);
                    }
                    setHasMore(data.meta.current_page < data.meta.last_page);
                    setPage(pageToLoad);
                }
            }
        } catch (e) {
            if (e.name !== 'AbortError') {
                console.error("Fetch error:", e);
            }
        }

        if (!currentController.signal.aborted) {
            setIsFetchingMore(false);
            if (pageToLoad === 1) {
                setProgress(1);
                setTimeout(() => setIsLoading(false), 300); // Small delay for animation
            }
        }

    }, [apiKey, search, category, selectedSeason, apiRequest, country, region]);

    // Clear teams immediately when filters change
    useEffect(() => {
        setTeams([]);
        setPage(1);
        setHasMore(true);
    }, [category, country, region]);

    // Initial load and filter changes
    useEffect(() => {
        const timer = setTimeout(() => {
            setPage(1);
            setHasMore(true);
            fetchTeams(1, true);
        }, 600); // Debounce team fetch
        return () => clearTimeout(timer);
    }, [fetchTeams]); // Note: fetchTeams depends on search, so this handles debouncing too

    const loadMore = () => {
        if (!isFetchingMore && hasMore && !loading) {
            setIsFetchingMore(true);
            fetchTeams(page + 1, false);
        }
    };

    const [refreshing, setRefreshing] = useState(false);

    const onRefresh = useCallback(() => {
        setRefreshing(true);
        setPage(1);
        setHasMore(true);
        fetchTeams(1, true).then(() => setRefreshing(false));
    }, [fetchTeams]);

    return (
        <View style={styles.viewContainer}>
            <CategoryFilter selected={category} onSelect={setCategory} />
            <View style={styles.searchHeader}>
                <View style={styles.searchBar}>
                    <Search size={18} color={THEME.colors.text.secondary} />
                    <TextInput
                        style={styles.searchInput}
                        placeholder={apiKey ? "Search Teams..." : "Demo Mode"}
                        placeholderTextColor={THEME.colors.text.secondary}
                        value={search}
                        onChangeText={setSearch}
                    />
                </View>
            </View>

            {/* Filters: Country & Region */}
            <View style={{ flexDirection: 'row', gap: 8, paddingHorizontal: 16, paddingBottom: 8, marginTop: 8, zIndex: 10 }}>
                <View style={{ flex: 1 }}>
                    <DropdownSelector
                        label="Country"
                        placeholder="All Countries"
                        value={country}
                        options={[{ label: 'Clear Filter', value: '' }, ...COUNTRIES]}
                        onSelect={(val) => {
                            setCountry(val);
                            setRegion(''); // Reset region when country changes
                        }}
                    />
                </View>
                <View style={{ flex: 1 }}>
                    <DropdownSelector
                        label="Region"
                        placeholder="All Regions"
                        value={region}
                        options={[{ label: 'Clear Filter', value: '' }, ...(country && REGIONS[country] ? REGIONS[country] : [])]}
                        onSelect={setRegion}
                        hasSearch={true}
                    />
                </View>
            </View>

            {isLoading && <LoadingScreen progress={progress} message="Loading Teams..." />}
            <FlatList
                data={teams}
                keyExtractor={item => item.id.toString()}
                contentContainerStyle={{ paddingBottom: 100 }}
                onEndReached={loadMore}
                onEndReachedThreshold={0.5}
                refreshControl={
                    <RefreshControl refreshing={refreshing} onRefresh={onRefresh} tintColor={THEME.colors.primary} />
                }
                ListFooterComponent={isFetchingMore ? <ActivityIndicator size="small" color={THEME.colors.primary} /> : null}
                renderItem={({ item, index }) => (
                    <RenderTeamItem
                        item={item}
                        index={index}
                        onSelectTeam={onSelectTeam}
                        styles={styles}
                        THEME={THEME}
                    />
                )}
            />
        </View >
    );
});

const SettingsView = React.memo(({ apiKey, setApiKey, seasons, selectedSeason, onSelectSeason }) => {
    const { theme: THEME, setTheme, themeName } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const [modalVisible, setModalVisible] = useState(false);

    return (
        <View style={styles.viewContainer}>
            <Modal
                animationType="slide"
                transparent={true}
                visible={modalVisible}
                onRequestClose={() => setModalVisible(false)}
            >
                <View style={{ flex: 1, backgroundColor: 'rgba(0,0,0,0.5)', justifyContent: 'flex-end' }}>
                    <View style={{ backgroundColor: THEME.colors.surface, borderTopLeftRadius: 20, borderTopRightRadius: 20, maxHeight: '80%' }}>
                        <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                            <Text style={{ fontSize: 18, fontWeight: 'bold', color: THEME.colors.text.primary }}>Select Season</Text>
                            <ScaleButton onPress={() => setModalVisible(false)} style={{ padding: 12 }}>
                                <X size={24} color={THEME.colors.text.secondary} />
                            </ScaleButton>
                        </View>
                        <FlatList
                            data={seasons}
                            keyExtractor={item => item.id.toString()}
                            renderItem={({ item }) => (
                                <ScaleButton
                                    style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between', padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}
                                    onPress={() => {
                                        onSelectSeason(item);
                                        setModalVisible(false);
                                    }}
                                >
                                    <Text style={{ fontSize: 16, color: selectedSeason?.id === item.id ? THEME.colors.primary : THEME.colors.text.primary, fontWeight: selectedSeason?.id === item.id ? '600' : '400' }}>{item.name}</Text>
                                    {selectedSeason?.id === item.id && <Check size={20} color={THEME.colors.primary} />}
                                </ScaleButton>
                            )}
                        />
                    </View>
                </View>
            </Modal>

            <ScrollView contentContainerStyle={{ paddingHorizontal: 20, paddingTop: 16, paddingBottom: 150 }}>
                <View style={styles.infoBox}>
                    <Info size={20} color={THEME.colors.primary} />
                    <View style={{ flex: 1 }}>
                        <Text style={styles.infoBoxTitle}>Real Data Mode</Text>
                    </View>
                </View>


                <Text style={styles.sectionTitle}>Appearance</Text>
                <View style={[styles.formCard, { marginBottom: 24 }]}>
                    <View style={styles.formItem}>
                        <Text style={styles.label}>Color Theme</Text>
                        <ScrollView horizontal showsHorizontalScrollIndicator={false} contentContainerStyle={{ gap: 12, marginTop: 8 }}>
                            {Object.values(THEMES).map(t => (
                                <ScaleButton key={t.id} onPress={() => setTheme(t.id)}>
                                    <View style={{ alignItems: 'center', gap: 6 }}>
                                        <View style={{
                                            width: 48, height: 48, borderRadius: 24,
                                            backgroundColor: t.colors.background,
                                            borderWidth: 2,
                                            borderColor: themeName === t.id ? THEME.colors.primary : t.colors.border,
                                            justifyContent: 'center', alignItems: 'center',
                                            overflow: 'hidden'
                                        }}>
                                            <View style={{ width: '50%', height: '100%', backgroundColor: t.colors.surface }} />
                                            <View style={{ position: 'absolute', width: 16, height: 16, borderRadius: 8, backgroundColor: t.colors.primary }} />
                                        </View>
                                        <Text style={{
                                            fontSize: 12,
                                            fontWeight: themeName === t.id ? 'bold' : '500',
                                            color: themeName === t.id ? THEME.colors.primary : THEME.colors.text.secondary
                                        }}>
                                            {t.name}
                                        </Text>
                                    </View>
                                </ScaleButton>
                            ))}
                        </ScrollView>
                    </View>
                </View>

                <Text style={styles.sectionTitle}>Season Configuration</Text>
                <ScaleButton onPress={() => setModalVisible(true)} style={[styles.formCard, { marginBottom: 24 }]}>
                    <View style={[styles.formItem, { flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' }]}>
                        <View>
                            <Text style={styles.label}>Active Season</Text>
                            <Text style={styles.input}>{selectedSeason?.name || 'Loading...'}</Text>
                        </View>
                        <ChevronRight size={20} color={THEME.colors.text.secondary} />
                    </View>
                </ScaleButton>

                <Text style={styles.sectionTitle}>API Configuration</Text>
                <View style={styles.formCard}>
                    <View style={styles.formItem}>
                        <Text style={styles.label}>RobotEvents API Key</Text>
                        <View style={{ flexDirection: 'row', alignItems: 'center', gap: 8 }}>
                            <View style={{ width: 8, height: 8, borderRadius: 4, backgroundColor: THEME.colors.success }} />
                            <Text style={{ fontSize: 14, color: THEME.colors.text.primary, fontWeight: '500' }}>Configured (Hidden)</Text>
                        </View>
                    </View>
                    <View style={styles.formFooter}>
                        <Text style={styles.formFooterText}>Data provided by RobotEvents API.</Text>
                    </View>
                </View>

                <Text style={[styles.sectionTitle, { marginTop: 32 }]}>MeowScore�?Ranks</Text>
                <View style={styles.formCard}>
                    <View style={styles.formItem}>
                        <Text style={styles.label}>What is MeowScore�?</Text>
                        <Text style={{ fontSize: 14, color: THEME.colors.text.secondary, marginTop: 8, lineHeight: 20 }}>
                            MeowScore�?is a comprehensive 0-100 rating that evaluates team strength based on win record, OPR, DPR, CCWM, AP, WP, and opponent strength.
                        </Text>
                    </View>
                    <View style={{ borderTopWidth: 1, borderTopColor: THEME.colors.border, paddingTop: 16, marginTop: 16 }}>
                        <Text style={[styles.label, { marginBottom: 12, paddingHorizontal: 16 }]}>Rank Order (Highest to Lowest)</Text>
                        {[
                            MEOW_RANKS.MASTER,
                            MEOW_RANKS.DIAMOND,
                            MEOW_RANKS.PLATINUM,
                            MEOW_RANKS.GOLD,
                            MEOW_RANKS.SILVER,
                            MEOW_RANKS.BRONZE,
                            MEOW_RANKS.IRON
                        ].map((rank, index) => (
                            <View key={rank.name} style={{
                                flexDirection: 'row',
                                alignItems: 'center',
                                paddingVertical: 8,
                                paddingHorizontal: 16,
                                borderTopWidth: index > 0 ? 1 : 0,
                                borderTopColor: THEME.colors.border + '40'
                            }}>
                                <Text style={{ fontSize: 18, marginRight: 12 }}>{rank.emoji}</Text>
                                <Text style={{
                                    fontSize: 14,
                                    fontWeight: 'bold',
                                    color: rank.color,
                                    flex: 1
                                }}>{rank.name}</Text>
                                <Text style={{
                                    fontSize: 12,
                                    color: THEME.colors.text.secondary
                                }}>{rank.min}-{rank.max}</Text>
                            </View>
                        ))}
                    </View>
                </View>
                <View style={{ alignItems: 'center', marginTop: 32, marginBottom: 16 }}>
                    <View style={{ flexDirection: 'row', alignItems: 'center', gap: 20, marginBottom: 16 }}>
                        <Image
                            source={require('./assets/makapaka_logo.png')}
                            style={{ width: 150, height: 150, borderRadius: 24 }}
                            resizeMode="contain"
                        />
                        <Image
                            source={STONEHENGE_LOGO}
                            style={{ width: 150, height: 150, resizeMode: 'contain' }}
                        />
                    </View>
                    <Text style={styles.versionText}>Makapaka Scout v1.3.1</Text>
                    <Text style={[styles.versionText, { fontSize: 12, marginTop: 8, fontWeight: '600', color: THEME.colors.primary }]}>
                        Designed and developed by Jeff J., Henry Z., and Steven Q.
                    </Text>
                    <Text style={[styles.versionText, { fontSize: 11, marginTop: 4 }]}>© 2025 Makapaka Scout. All rights reserved.</Text>
                    <Text style={[styles.versionText, { fontSize: 10, marginTop: 8 }]}>Data provided by RobotEvents API</Text>
                </View>
            </ScrollView>
        </View>
    );
});


const EventDetailView = React.memo(({ event, apiKey, apiRequest, pushView, isFavorite, onToggleFavorite }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const [activeTab, setActiveTab] = useState('info');
    const fadeAnim = useRef(new Animated.Value(1)).current;
    const slideAnim = useRef(new Animated.Value(0)).current;
    const [data, setData] = useState({ teams: [], skills: [], divisions: [] });
    const [loading, setLoading] = useState(false);

    const [refreshing, setRefreshing] = useState(false);
    const [progress, setProgress] = useState(0);

    const fetchAllPages = useCallback(async (endpoint) => {
        let allItems = [];
        const PER_PAGE = 250;
        const firstResponse = await apiRequest(endpoint, { page: 1, per_page: PER_PAGE });
        if (firstResponse && firstResponse.data) {
            allItems = [...firstResponse.data];
            const lastPage = firstResponse.meta.last_page;
            for (let p = 2; p <= lastPage; p++) {
                setProgress(p / lastPage);
                const response = await apiRequest(endpoint, { page: p, per_page: PER_PAGE });
                if (response && response.data) {
                    allItems = [...allItems, ...response.data];
                }
            }
        }
        return allItems;
    }, [apiRequest]);

    const fetchData = useCallback(async (isRefresh = false) => {
        if (!apiKey || activeTab === 'info') return;

        // Check if we already have data to avoid re-fetching unless refreshing
        if (!isRefresh && (activeTab === 'teams' || activeTab === 'divisions') && data.teams.length > 0) return;
        if (!isRefresh && activeTab === 'skills' && data.skills.length > 0 && data.teams.length > 0) return;

        setLoading(true);
        setProgress(0);
        try {
            if (activeTab === 'teams' || activeTab === 'divisions') {
                const teams = await fetchAllPages(`/events/${event.id}/teams`);
                setData(prev => ({ ...prev, teams }));
            } else if (activeTab === 'skills') {
                // Fetch skills
                let skills = [];
                // If refreshing, always fetch. If not, use existing if available (though logic above handles this)
                skills = await fetchAllPages(`/events/${event.id}/skills`);

                // Fetch teams if missing (needed for names)
                let teams = data.teams;
                if (teams.length === 0 || isRefresh) {
                    teams = await fetchAllPages(`/events/${event.id}/teams`);
                }

                setData(prev => ({ ...prev, skills, teams }));
            }
        } catch (e) {
            console.error("Failed to fetch event details:", e);
        } finally {
            setLoading(false);
            setRefreshing(false);
            setProgress(1);
        }
    }, [activeTab, event.id, apiKey, fetchAllPages, data.teams.length, data.skills.length]);

    useEffect(() => {
        fetchData();
    }, [fetchData]);

    const onRefresh = useCallback(() => {
        setRefreshing(true);
        fetchData(true);
    }, [fetchData]);

    const tabs = [
        { id: 'info', label: 'Information' },
        { id: 'teams', label: 'Teams' },
        { id: 'skills', label: 'Skills Rankings' },
        { id: 'divisions', label: 'Divisions' },
    ];

    const processedSkills = useMemo(() => {
        if (!data.skills || data.skills.length === 0) return [];

        const teamMap = {};

        data.skills.forEach(item => {
            const teamId = item.team.id;
            if (!teamMap[teamId]) {
                teamMap[teamId] = {
                    team: item.team,
                    driver: 0,
                    driverAttempts: 0,
                    programming: 0,
                    programmingAttempts: 0,
                    combined: 0
                };
            }

            if (item.type === 'driver') {
                teamMap[teamId].driver = item.score;
                teamMap[teamId].driverAttempts = item.attempts;
            } else if (item.type === 'programming') {
                teamMap[teamId].programming = item.score;
                teamMap[teamId].programmingAttempts = item.attempts;
            }
        });

        const result = Object.values(teamMap).map(t => ({
            ...t,
            combined: t.driver + t.programming
        }));

        return result.sort((a, b) => b.combined - a.combined);
    }, [data.skills]);

    const teamNameMap = useMemo(() => {
        const map = {};
        if (data.teams) {
            data.teams.forEach(t => {
                map[t.id] = t.team_name;
            });
        }
        return map;
    }, [data.teams]);

    const renderContent = () => {
        const divisionsData = event.divisions && event.divisions.length > 0
            ? event.divisions
            : [{ id: 'default', name: 'Default' }];

        return (
            <View style={{ flex: 1 }}>
                {loading && <LoadingScreen progress={progress} message={`Loading ${activeTab}...`} />}

                <Animated.View style={{
                    flex: 1,
                    opacity: fadeAnim,
                    transform: [{ translateX: slideAnim }],
                    display: activeTab === 'info' ? 'flex' : 'none'
                }}>
                    <ScrollView contentContainerStyle={{ padding: 16, paddingBottom: 100 }}>
                        <Text style={{ fontSize: 24, fontWeight: 'bold', marginBottom: 8, color: THEME.colors.text.primary }}>{event.name}</Text>
                        <View style={styles.card}>
                            <View style={styles.formItem}>
                                <Text style={styles.label}>Date</Text>
                                <Text style={styles.input}>{new Date(event.start).toLocaleDateString()} - {new Date(event.end).toLocaleDateString()}</Text>
                            </View>
                        </View>
                        <View style={styles.card}>
                            <View style={styles.formItem}>
                                <Text style={styles.label}>Location</Text>
                                <Text style={styles.input}>{event.location?.venue}</Text>
                                <Text style={styles.input}>{event.location?.address_1}</Text>
                                <Text style={styles.input}>{event.location?.city}, {event.location?.region}, {event.location?.country}</Text>
                            </View>
                        </View>
                        <View style={styles.card}>
                            <View style={styles.formItem}>
                                <Text style={styles.label}>Program</Text>
                                <Text style={styles.input}>{event.program?.name}</Text>
                            </View>
                        </View>
                        <View style={styles.card}>
                            <View style={styles.formItem}>
                                <Text style={styles.label}>MeowScore�?Tier</Text>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginTop: 4 }}>
                                    <View style={{
                                        paddingHorizontal: 12, paddingVertical: 4, borderRadius: 12,
                                        backgroundColor: getEventTier(event).color, marginRight: 8
                                    }}>
                                        <Text style={{ fontWeight: 'bold', color: 'white', fontSize: 14 }}>
                                            {getEventTier(event).name}
                                        </Text>
                                    </View>
                                    <Text style={{ color: THEME.colors.text.secondary }}>
                                        Swing: {getEventTier(event).swing > 0 ? '+' : ''}{getEventTier(event).swing}
                                    </Text>
                                </View>
                            </View>
                        </View>
                    </ScrollView>
                </Animated.View>

                <Animated.View style={{
                    flex: 1,
                    opacity: fadeAnim,
                    transform: [{ translateX: slideAnim }],
                    display: activeTab === 'teams' ? 'flex' : 'none'
                }}>
                    <FlatList
                        data={data.teams}
                        keyExtractor={item => item.id.toString()}
                        contentContainerStyle={{ padding: 16, paddingBottom: 120 }}
                        renderItem={({ item }) => (
                            <View style={styles.listItem}>
                                <View>
                                    <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.primary }}>{item.number}</Text>
                                    <Text style={{ color: THEME.colors.text.primary }}>{item.team_name}</Text>
                                    <Text style={{ color: THEME.colors.text.secondary, fontSize: 12 }}>{item.location?.city}, {item.location?.region}</Text>
                                </View>
                            </View>
                        )}
                        ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No teams found.</Text>}
                        refreshControl={<RefreshControl refreshing={refreshing} onRefresh={onRefresh} tintColor={THEME.colors.primary} />}
                    />
                </Animated.View>

                <Animated.View style={{
                    flex: 1,
                    opacity: fadeAnim,
                    transform: [{ translateX: slideAnim }],
                    display: activeTab === 'skills' ? 'flex' : 'none'
                }}>
                    <FlatList
                        data={processedSkills}
                        keyExtractor={item => item.team.id.toString()}
                        contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                        renderItem={({ item, index }) => (
                            <View style={styles.listItem}>
                                <View style={{ flex: 1, flexDirection: 'row', alignItems: 'center', gap: 12, marginRight: 8 }}>
                                    <Text style={{ fontWeight: 'bold', width: 24, textAlign: 'center', color: THEME.colors.text.primary }}>{index + 1}</Text>
                                    <View style={{ flex: 1 }}>
                                        <Text style={{ fontWeight: 'bold', color: THEME.colors.primary }}>{item.team.name}</Text>
                                        <Text style={{ fontSize: 12, color: THEME.colors.text.secondary }} numberOfLines={1} ellipsizeMode="tail">
                                            {teamNameMap[item.team.id] || item.team.team_name || ''}
                                        </Text>
                                    </View>
                                </View>
                                <View style={{ alignItems: 'flex-end', minWidth: 80 }}>
                                    <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.text.primary }}>{item.combined}</Text>
                                    <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>
                                        Driver: {item.driver} ({item.driverAttempts})
                                    </Text>
                                    <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>
                                        Prog: {item.programming} ({item.programmingAttempts})
                                    </Text>
                                </View>
                            </View>
                        )}
                        ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No skills data found.</Text>}
                        refreshControl={<RefreshControl refreshing={refreshing} onRefresh={onRefresh} tintColor={THEME.colors.primary} />}
                    />
                </Animated.View>

                <Animated.View style={{
                    flex: 1,
                    opacity: fadeAnim,
                    transform: [{ translateX: slideAnim }],
                    display: activeTab === 'divisions' ? 'flex' : 'none'
                }}>
                    <ScrollView contentContainerStyle={{ padding: 16, paddingBottom: 120 }}>
                        {/* Division Predictor Tool */}
                        <DivisionPredictor
                            teams={data.teams}
                            onRefreshTeams={() => fetchData(true)}
                        />

                        <Text style={{ fontSize: 18, fontWeight: 'bold', marginTop: 24, marginBottom: 8, color: THEME.colors.text.primary }}>
                            Official Divisions
                        </Text>

                        {divisionsData.map((item) => (
                            <ScaleButton
                                key={item.id}
                                style={[styles.listItem, { marginBottom: 8 }]}
                                onPress={() => pushView('divisionDetail', { event, division: item })}
                            >
                                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between', flex: 1 }}>
                                    <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.text.primary }}>{item.name}</Text>
                                    <ChevronRight color={THEME.colors.text.secondary} size={20} />
                                </View>
                            </ScaleButton>
                        ))}

                        {divisionsData.length === 0 && (
                            <Text style={{ textAlign: 'center', marginTop: 8, color: THEME.colors.text.secondary }}>
                                No officials divisions found.
                            </Text>
                        )}
                    </ScrollView>
                </Animated.View>
            </View>
        );
    };

    return (
        <View style={styles.viewContainer}>
            <View style={{ padding: 16, backgroundColor: THEME.colors.surface, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'flex-start' }}>
                    <View style={{ flex: 1 }}>
                        <Text style={{ fontSize: 20, fontWeight: 'bold', color: THEME.colors.text.primary }}>{event.name}</Text>
                        <Text style={{ color: THEME.colors.text.secondary, marginTop: 4 }}>{event.location?.venue}, {event.location?.city}</Text>
                    </View>
                    <View style={{ flexDirection: 'row' }}>
                        <ScaleButton onPress={() => openRobotEvents('event', event)} style={{ padding: 12 }}>
                            <View pointerEvents="none">
                                <ExternalLink color={THEME.colors.primary} size={24} />
                            </View>
                        </ScaleButton>
                        <ScaleButton onPress={() => onToggleFavorite(event, 'event')} style={{ padding: 12 }}>
                            <View pointerEvents="none">
                                <Star color={isFavorite ? THEME.colors.primary : THEME.colors.text.secondary} fill={isFavorite ? THEME.colors.primary : 'none'} size={24} />
                            </View>
                        </ScaleButton>
                    </View>
                </View>
            </View>
            <View style={{ flexDirection: 'row', backgroundColor: THEME.colors.background, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                {tabs.map(tab => (
                    <ScaleButton
                        key={tab.id}
                        onPress={() => {
                            if (activeTab !== tab.id) {
                                Animated.sequence([
                                    Animated.parallel([
                                        Animated.timing(fadeAnim, {
                                            toValue: 0,
                                            duration: 150,
                                            useNativeDriver: true,
                                        }),
                                        Animated.timing(slideAnim, {
                                            toValue: 30,
                                            duration: 150,
                                            useNativeDriver: true,
                                        }),
                                    ]),
                                ]).start(() => {
                                    setActiveTab(tab.id);
                                    slideAnim.setValue(-30);
                                    Animated.parallel([
                                        Animated.timing(fadeAnim, {
                                            toValue: 1,
                                            duration: 200,
                                            useNativeDriver: true,
                                        }),
                                        Animated.timing(slideAnim, {
                                            toValue: 0,
                                            duration: 200,
                                            useNativeDriver: true,
                                        }),
                                    ]).start();
                                });
                            }
                        }}
                        style={{
                            flex: 1,
                            paddingVertical: 12,
                            alignItems: 'center',
                            borderBottomWidth: 2,
                            borderBottomColor: activeTab === tab.id ? THEME.colors.primary : 'transparent'
                        }}
                        activeScale={0.9}
                    >
                        <Text style={{
                            color: activeTab === tab.id ? THEME.colors.primary : THEME.colors.text.secondary,
                            fontWeight: activeTab === tab.id ? '600' : '400',
                            fontSize: 12, // Smaller font to fit 4 tabs
                            textTransform: 'uppercase'
                        }}>
                            {tab.label}
                        </Text>
                    </ScaleButton>
                ))}
            </View>
            {renderContent()}
        </View>
    );
});

// --- Helper for RobotEvents Links ---
const openRobotEvents = (type, data) => {
    let url = '';
    if (type === 'team') {
        const program = data.program?.code || 'VRC';
        url = `https://www.robotevents.com/teams/${program}/${data.number}`;
    } else if (type === 'event') {
        let programSlug = 'vex-robotics-competition';
        const code = data.program?.code;
        if (code === 'VIQC') programSlug = 'vex-iq-competition';
        else if (code === 'VEXU') programSlug = 'vex-u';
        else if (code === 'VAIRC') programSlug = 'vex-ai-robotics-competition';

        url = `https://www.robotevents.com/robot-competitions/${programSlug}/${data.sku}.html`;
    }

    if (url) {
        Linking.openURL(url).catch(err => console.error("Couldn't load page", err));
    }
};

// --- Team Info View (Simple, no event context) ---

// --- Global Cache for Season Stats (Prevents 429 Loops) ---
const SEASON_STATS_CACHE = {};
const SEASON_STATS_PROMISES = {};

const TeamInfoView = React.memo(({ team: propTeam, event, division, apiKey, apiRequest, pushView, selectedSeason, isFavorite, onToggleFavorite }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const [fetchedTeam, setFetchedTeam] = useState(null);
    const team = fetchedTeam || propTeam;

    const [activeTab, setActiveTab] = useState('info');
    const fadeAnim = useRef(new Animated.Value(1)).current;
    const slideAnim = useRef(new Animated.Value(0)).current;
    const [seasonStats, setSeasonStats] = useState({ wins: 0, losses: 0, ties: 0, avgScore: 0, isVexIQ: false, opr: 0, dpr: 0, ccwm: 0, ap: 0, wp: 0 });
    const [awards, setAwards] = useState([]);
    const [events, setEvents] = useState([]);
    const [skills, setSkills] = useState(null); // World skills data
    const [loading, setLoading] = useState(true);
    const [meowScore, setMeowScore] = useState(null);
    const [simulate, setSimulate] = useState(false);

    // Event specific data
    const [eventMatches, setEventMatches] = useState([]);
    const [eventRanking, setEventRanking] = useState(null);
    const [eventStats, setEventStats] = useState({});
    const [allEventStats, setAllEventStats] = useState({});
    const [allSeasonRankings, setAllSeasonRankings] = useState({});
    const [refreshing, setRefreshing] = useState(false);
    const isVexIQ = team.program?.id === 41 || team.program?.code === 'VIQC';

    // Fetch full team details if missing (e.g. navigation from Match Detail)
    useEffect(() => {
        if (!team.location || !team.program) {
            apiRequest(`/teams/${team.id}`).then(data => {
                if (data) setFetchedTeam(data);
            }).catch(console.error);
        }
    }, [team.id, team.location, team.program, apiRequest]);

    // Automatic Season Selection Logic
    const [effectiveSeason, setEffectiveSeason] = useState(selectedSeason);

    useEffect(() => {
        const determineSeason = async () => {
            if (!team || !selectedSeason) return;

            // If team program matches selected season program, use selected season
            if (team.program && selectedSeason.program && team.program.id === selectedSeason.program.id) {
                setEffectiveSeason(selectedSeason);
                return;
            }

            // Mismatch detected: Fetch active season for the team's program
            if (team.program?.id) {
                try {
                    // console.log(`Program mismatch! Team ${team.number} is ${team.program.code}, Season is ${selectedSeason.program?.code}. Fetching correct season...`);
                    const seasonsData = await apiRequest('/seasons', {
                        program: [team.program.id],
                        active: true // Get active seasons only
                    });

                    if (seasonsData && Array.isArray(seasonsData.data) && seasonsData.data.length > 0) {
                        // Sort by ID desc (newest first) and pick top
                        const correctSeason = seasonsData.data.sort((a, b) => b.id - a.id)[0];
                        setEffectiveSeason(correctSeason);
                        // console.log(`Switched context to season: ${correctSeason.name}`);
                    } else {
                        // Fallback: Just keep selected (avoids crash, but might show empty data)
                        setEffectiveSeason(selectedSeason);
                    }
                } catch (e) {
                    console.error("Failed to auto-select season:", e);
                    setEffectiveSeason(selectedSeason);
                }
            }
        };

        determineSeason();
    }, [team.id, selectedSeason?.id, team.program?.id, apiRequest]);

    const fetchSeasonData = useCallback(async () => {
        // Use effectiveSeason instead of selectedSeason
        if (!apiKey || !effectiveSeason) return;

        setLoading(true);
        try {
            // Fetch awards, matches, events, skills, and rankings in parallel
            const [awardsResponse, matchesResponse, eventsResponse, skillsResponse, rankingsResponse] = await Promise.all([
                apiRequest(`/teams/${team.id}/awards`, {
                    season: effectiveSeason.id,
                    per_page: 250
                }),
                apiRequest(`/teams/${team.id}/matches`, {
                    season: effectiveSeason.id,
                    per_page: 250
                }),
                apiRequest(`/teams/${team.id}/events`, {
                    season: effectiveSeason.id,
                    per_page: 250
                }),
                apiRequest(`/teams/${team.id}/skills`, {
                    season: effectiveSeason.id
                }),
                apiRequest(`/teams/${team.id}/rankings`, {
                    season: effectiveSeason.id,
                    per_page: 250
                })
            ]);

            console.log("Global Skills Response for team", team.id, "Searching via Binary Search...");

            if (awardsResponse && Array.isArray(awardsResponse.data)) {
                setAwards(awardsResponse.data);
            } else {
                setAwards([]);
            }

            if (eventsResponse && Array.isArray(eventsResponse.data)) {
                // Sort events by start date descending (latest first)
                const sortedEvents = eventsResponse.data.sort((a, b) => new Date(b.start) - new Date(a.start));
                setEvents(sortedEvents);
            } else {
                setEvents([]);
            }

            // Process skills data - get highest official combined score from a single event
            let combinedScore = 0;
            let bestDriverScore = 0;
            let bestProgrammingScore = 0;

            if (skillsResponse && Array.isArray(skillsResponse.data) && skillsResponse.data.length > 0) {
                // The API returns individual runs. We need to find the best *combined* entry if available, 
                // but the /teams/{id}/skills endpoint returns individual run scores (driver/programming), not combined rankings.
                // However, the /seasons/{id}/skills endpoint (which we use for global rank) DOES return combined scores.

                // WAIT: The previous logic was flawd because it mixed events.
                // We need to group by event and find the best combined score PER EVENT.
                const eventSkills = {};

                skillsResponse.data.forEach(run => {
                    const eventId = run.event.id;
                    if (!eventSkills[eventId]) {
                        eventSkills[eventId] = { driver: 0, programming: 0 };
                    }
                    if (run.type === 'driver') {
                        eventSkills[eventId].driver = Math.max(eventSkills[eventId].driver, run.score);
                    } else if (run.type === 'programming') {
                        eventSkills[eventId].programming = Math.max(eventSkills[eventId].programming, run.score);
                    }
                });

                // Find the event with the highest total
                Object.values(eventSkills).forEach(scores => {
                    const total = scores.driver + scores.programming;
                    if (total > combinedScore) {
                        combinedScore = total;
                        bestDriverScore = scores.driver;
                        bestProgrammingScore = scores.programming;
                    }
                });
            }

            // Find Global Rank via Binary Search if we have a score
            let globalRank = null;
            if (combinedScore > 0) {
                try {
                    const findTeamRank = async () => {
                        const endpoint = `https://www.robotevents.com/api/seasons/${effectiveSeason.id}/skills`;
                        const params = { grade_level: team.grade_level, per_page: 250 };

                        // 1. Fetch Page 1
                        const page1 = await apiRequest(endpoint, { ...params, page: 1 });
                        if (!page1) return null;

                        const list = Array.isArray(page1) ? page1 : (page1.data || []);
                        const meta = page1.meta || {};
                        const lastPage = meta.last_page || 1;

                        // Check Page 1
                        const foundP1 = list.find(r => r.team.id === team.id);
                        if (foundP1) return foundP1.rank;

                        if (lastPage === 1) return null;

                        // Binary Search Pages
                        let low = 2;
                        let high = lastPage;
                        let foundRank = null;

                        while (low <= high && !foundRank) {
                            const mid = Math.floor((low + high) / 2);
                            const pData = await apiRequest(endpoint, { ...params, page: mid });
                            const pList = Array.isArray(pData) ? pData : (pData.data || []);

                            if (!pList.length) { high = mid - 1; continue; }

                            const found = pList.find(r => r.team.id === team.id);
                            if (found) {
                                foundRank = found.rank;
                                break;
                            }

                            const maxScore = pList[0].score;
                            const minScore = pList[pList.length - 1].score;

                            if (combinedScore > maxScore) {
                                high = mid - 1;
                            } else if (combinedScore < minScore) {
                                low = mid + 1;
                            } else {
                                // Score is within range but team not found.
                                // Could be a tiebreaker boundary issue. Check neighbors.
                                // Optimization: If on boundary, allow search to continue
                                if (combinedScore === maxScore) high = mid - 1;
                                else if (combinedScore === minScore) low = mid + 1;
                                else break; // Strictly inside and not found -> missing
                            }
                        }
                        return foundRank;
                    };

                    globalRank = await findTeamRank();
                } catch (e) {
                    console.error("Error finding global rank:", e);
                }
            }

            setSkills({
                driver: bestDriverScore,
                programming: bestProgrammingScore,
                score: combinedScore,
                rank: globalRank
            });


            // Calculate W-L-T and gather stats from matches/events for Meow Score
            let wins = 0;
            let losses = 0;
            let ties = 0;
            let totalScore = 0;
            let matchCount = 0;
            let totalAP = 0;
            let totalWP = 0;
            const isVexIQ = team.program?.id === 41;

            // Extract AP and WP from rankings
            if (rankingsResponse && Array.isArray(rankingsResponse.data)) {
                rankingsResponse.data.forEach(ranking => {
                    // Sum up AP (Autonomous Win Points) and WP (Win Points) across events
                    totalAP += ranking.ap || 0;
                    totalWP += ranking.wp || 0;
                });
            }

            // Arrays to collect stats from each event for averaging
            const eventStatsList = [];

            if (matchesResponse && Array.isArray(matchesResponse.data)) {
                let allMatches = matchesResponse.data;

                // Handle pagination if needed
                if (matchesResponse.meta && matchesResponse.meta.last_page > 1) {
                    const promises = [];
                    for (let p = 2; p <= matchesResponse.meta.last_page; p++) {
                        promises.push(apiRequest(`/teams/${team.id}/matches`, {
                            season: effectiveSeason.id, // Use effectiveSeason
                            per_page: 250,
                            page: p
                        }));
                    }
                    const moreMatches = await Promise.all(promises);
                    moreMatches.forEach(m => {
                        if (m && m.data) allMatches = [...allMatches, ...m.data];
                    });
                }

                // Calculate W-L-T
                allMatches.forEach(match => {
                    if (!match.alliances) return;
                    const redAlliance = match.alliances.find(a => a.color === 'red');
                    const blueAlliance = match.alliances.find(a => a.color === 'blue');

                    // Skip if scores are 0-0 (likely unplayed)
                    if (redAlliance.score === 0 && blueAlliance.score === 0) return;

                    // Ensure alliance.teams exists
                    const redTeams = redAlliance.teams || [];
                    const blueTeams = blueAlliance.teams || [];

                    const isRed = redTeams.some(t => t.team.id === team.id);
                    const isBlue = blueTeams.some(t => t.team.id === team.id);

                    if (isVexIQ) {
                        if (isRed) {
                            totalScore += redAlliance.score;
                            matchCount++;
                        } else if (isBlue) {
                            totalScore += blueAlliance.score;
                            matchCount++;
                        }
                    } else {
                        if (isRed) {
                            if (redAlliance.score > blueAlliance.score) wins++;
                            else if (redAlliance.score < blueAlliance.score) losses++;
                            else ties++;
                        } else if (isBlue) {
                            if (blueAlliance.score > redAlliance.score) wins++;
                            else if (blueAlliance.score < redAlliance.score) losses++;
                            else ties++;
                        }
                    }
                });

                // Calculate OPR/DPR/CCWM per event for averaging
                const eventMeowScores = [];

                if (eventsResponse && eventsResponse.data) {
                    for (const event of eventsResponse.data) {
                        try {
                            // Get matches for this event (filtering from allMatches which we already fetched)
                            const eventMatches = allMatches.filter(m => m.event.id === event.id);

                            if (eventMatches.length > 0) {
                                // 1. Calculate OPR (Existing Logic)
                                const teamsInEvent = new Set();
                                let eWins = 0, eLosses = 0, eTies = 0, eAWP = 0;
                                let eTotalScore = 0, eTotalOppScore = 0;

                                eventMatches.forEach(match => {
                                    if (match.alliances) {
                                        // OPR Helpers
                                        match.alliances.forEach(alliance => {
                                            if (alliance.teams) {
                                                alliance.teams.forEach(t => teamsInEvent.add(t.team.id));
                                            }
                                        });

                                        // MeowScore Helpers
                                        // Check if this match is "Scored" (played)
                                        // Some matches might not have 'scored' flag but have scores.
                                        const isRed = match.alliances.some(a => a.color === 'red' && a.teams.some(t => t.team.id === team.id));
                                        const myAlliance = match.alliances.find(a => a.color === (isRed ? 'red' : 'blue'));
                                        const oppAlliance = match.alliances.find(a => a.color === (isRed ? 'blue' : 'red'));

                                        if (!match.scored) {
                                            // Fallback: If score is 0-0, assume unplayed. If score > 0, assume played.
                                            if (myAlliance && oppAlliance && myAlliance.score === 0 && oppAlliance.score === 0) return;
                                        }

                                        if (myAlliance && oppAlliance) {
                                            const myScore = myAlliance.score;
                                            const oppScore = oppAlliance.score;
                                            eTotalScore += myScore;
                                            eTotalOppScore += oppScore;

                                            let resultPoints = 0;
                                            if (myScore > oppScore) { eWins++; resultPoints = 2; }
                                            else if (myScore < oppScore) { eLosses++; resultPoints = 0; }
                                            else { eTies++; resultPoints = 1; }

                                            // AWP Calculation
                                            const earnedWP = myAlliance.wp || 0;
                                            if (earnedWP > resultPoints) {
                                                eAWP++;
                                            }
                                        }
                                    }
                                });

                                const teamsArray = Array.from(teamsInEvent).map(id => ({ id }));

                                // Calculate stats for this event (for OPR averaging)
                                const stats = calculateStats(eventMatches, teamsArray);
                                if (stats[team.id]) {
                                    eventStatsList.push(stats[team.id]);
                                }

                                // 2. Calculate Meow Event Score
                                const eventStats = {
                                    wins: eWins, losses: eLosses, ties: eTies,
                                    awp: eAWP,
                                    programming: bestProgrammingScore || 0,
                                    opr: eWins + eLosses + eTies > 0 ? eTotalScore / (eWins + eLosses + eTies) : 0, // Fallback simple OPR
                                    // We don't need real OPR for calculateEventScore based on current heuristic
                                    isVexIQ: isVexIQ
                                };

                                const { scoreDelta } = calculateEventScore(eventStats, event);
                                eventMeowScores.push(50 + scoreDelta);
                            }
                        } catch (err) {
                            console.error('Error calculating stats for event:', event.id, err);
                        }
                    }
                }

                // Average OPR, DPR, CCWM across events
                let avgOPR = 0, avgDPR = 0, avgCCWM = 0;
                if (eventStatsList.length > 0) {
                    const sum = eventStatsList.reduce((acc, stats) => ({
                        opr: acc.opr + (stats.opr || 0),
                        dpr: acc.dpr + (stats.dpr || 0),
                        ccwm: acc.ccwm + (stats.ccwm || 0)
                    }), { opr: 0, dpr: 0, ccwm: 0 });

                    avgOPR = sum.opr / eventStatsList.length;
                    avgDPR = sum.dpr / eventStatsList.length;
                    avgCCWM = sum.ccwm / eventStatsList.length;
                }

                const avgScore = matchCount > 0 ? (totalScore / matchCount).toFixed(1) : 0;

                // Set season stats with all data needed for Meow Score
                setSeasonStats({
                    wins,
                    losses,
                    ties,
                    avgScore,
                    isVexIQ,
                    opr: avgOPR,
                    dpr: avgDPR,
                    ccwm: avgCCWM,
                    ap: totalAP,
                    wp: totalWP
                });

                // Calculate Meow Score (Average of Event Scores)
                // Fallback to old calc if no events processed? 
                // If events exist, use new method.
                let finalScore = 0;
                if (eventMeowScores.length > 0) {
                    const totalMeow = eventMeowScores.reduce((a, b) => a + b, 0);
                    finalScore = Math.round(totalMeow / eventMeowScores.length);
                    finalScore = Math.min(100, Math.max(0, finalScore)); // Clamp
                } else {
                    // Fallback to old method if something fails or no events found locally
                    finalScore = calculateMeowScore({
                        wins,
                        losses,
                        ties,
                        opr: avgOPR,
                        dpr: avgDPR,
                        ccwm: avgCCWM,
                        ap: totalAP,
                        wp: totalWP,
                        isVexIQ
                    });
                }

                setMeowScore(finalScore);

            }
        } catch (e) {
            console.error("Failed to fetch season data:", e);
            setSeasonStats({ wins: 0, losses: 0, ties: 0, avgScore: 0, isVexIQ: false, opr: 0, dpr: 0, ccwm: 0, ap: 0, wp: 0 });
            setMeowScore(null);
        } finally {
            setLoading(false);
            setRefreshing(false);
        }
    }, [team.id, effectiveSeason?.id, apiKey, apiRequest]);

    // Fetch Event Specific Data
    useEffect(() => {
        if (!event || !division || !apiKey) return;

        const fetchEventData = async (background = false) => {
            try {
                // Fetch matches for this team at this event/division
                let allMatches = [];
                const response = await apiRequest(`/events/${event.id}/divisions/${division.id}/matches`, { per_page: 250 });
                if (response && response.data) {
                    allMatches = response.data;
                    // Fetch additional pages if needed
                    const lastPage = response.meta?.last_page || 1;
                    for (let p = 2; p <= lastPage; p++) {
                        const pageRes = await apiRequest(`/events/${event.id}/divisions/${division.id}/matches`, { page: p, per_page: 250 });
                        if (pageRes && pageRes.data) {
                            allMatches = [...allMatches, ...pageRes.data];
                        }
                    }
                }

                // Filter matches that include this team
                const teamMatches = allMatches.filter(match =>
                    match.alliances?.some(alliance =>
                        alliance.teams?.some(t => t.team.id === team.id)
                    )
                );

                // Sort matches chronologically
                const sortMatches = (matches) => {
                    return matches.sort((a, b) => {
                        const getMatchPriority = (name) => {
                            if (!name) return 999;
                            const upper = name.toUpperCase();
                            if (upper.startsWith('P')) return 0;
                            if (upper.startsWith('Q')) {
                                if (upper.includes('QUARTER') || upper.startsWith('QF')) return 3;
                                return 1;
                            }
                            if (upper.startsWith('T')) return 1; // Teamwork -> Qualification
                            if (upper.startsWith('R16')) return 2;
                            if (upper.startsWith('S')) return 4;
                            if (upper.startsWith('F')) return 5;
                            return 6;
                        };

                        const getMatchNumber = (name) => {
                            const match = name?.match(/\d+/);
                            return match ? parseInt(match[0]) : 0;
                        };

                        const aPriority = getMatchPriority(a.name);
                        const bPriority = getMatchPriority(b.name);

                        if (aPriority !== bPriority) {
                            return aPriority - bPriority;
                        }

                        return getMatchNumber(a.name) - getMatchNumber(b.name);
                    });
                };

                setEventMatches(sortMatches(teamMatches));

                // Fetch rankings
                const rankRes = await apiRequest(`/events/${event.id}/divisions/${division.id}/rankings`, { per_page: 250 });
                if (rankRes && rankRes.data) {
                    const teamRank = rankRes.data.find(r => r.team.id === team.id);
                    setEventRanking(teamRank);

                    // Calculate stats if we have matches
                    if (allMatches.length > 0) {
                        const teams = rankRes.data.map(r => r.team);
                        const calculatedStats = calculateStats(allMatches, teams);
                        setAllEventStats(calculatedStats);
                        if (calculatedStats[team.id]) {
                            setEventStats(calculatedStats[team.id]);
                        }
                    }
                }
            } catch (e) {
                console.error("Failed to fetch team event data:", e);
            }
        };

        fetchEventData();

        // Live update interval (3 seconds)
        const intervalId = setInterval(() => {
            if (activeTab === 'matches') {
                // console.log("Auto-refreshing team match data...");
                fetchEventData(true);
            }
        }, 3000);

        return () => clearInterval(intervalId);

    }, [team.id, event?.id, division?.id, apiKey, apiRequest, activeTab]);

    // Separate Effect for fetching HEAVY season data (Global Stats)
    // Runs ONLY once when event/season changes, NOT on interval
    useEffect(() => {
        if (!event || !division || !apiKey || !selectedSeason) return;

        const cacheKey = `${event.id}_${selectedSeason.id}`;

        // 1. Check Cache (Instant)
        if (SEASON_STATS_CACHE[cacheKey]) {
            // console.log("Using cached season stats for current event.");
            setAllSeasonRankings(SEASON_STATS_CACHE[cacheKey]);
            return;
        }

        // 2. Check Active Promise (Join the train)
        if (SEASON_STATS_PROMISES[cacheKey]) {
            // console.log("Joining existing season stats fetch...");
            SEASON_STATS_PROMISES[cacheKey].then(data => {
                if (data) setAllSeasonRankings(data);
            });
            return;
        }

        // 3. Start New Fetch (I am the driver)
        const fetchGlobalSeasonStats = async () => {
            // console.log("Starting fresh season stats fetch...");
            let fullRankingsMap = {};
            try {
                // First get list of teams in event from rankings (lightweight)
                const rankRes = await apiRequest(`/events/${event.id}/divisions/${division.id}/rankings`, { per_page: 250 });

                if (rankRes && rankRes.data) {
                    const teamIds = rankRes.data.map(r => r.team.id);

                    // Throttle settings: Concurrency 3, Delay 1200ms
                    // Very safe for avoiding 429
                    const chunkSize = 3;

                    // We iterate through chunks
                    for (let i = 0; i < teamIds.length; i += chunkSize) {
                        const chunk = teamIds.slice(i, i + chunkSize);
                        let chunkMap = {};

                        const fetchTeamMatches = async (teamId) => {
                            try {
                                const matchesRes = await apiRequest(`/teams/${teamId}/matches`, {
                                    season: selectedSeason.id,
                                    round: 2, // Quals
                                    per_page: 250
                                });

                                if (matchesRes && matchesRes.data) {
                                    const matches = matchesRes.data;

                                    if (!chunkMap[teamId]) {
                                        chunkMap[teamId] = { wins: 0, losses: 0, ties: 0, wp: 0, matches: 0 };
                                    }

                                    matches.forEach(m => {
                                        if (m.score < 1 && m.alliances[0].score === 0 && m.alliances[1].score === 0) return;

                                        const red = m.alliances.find(a => a.color === 'red');
                                        const blue = m.alliances.find(a => a.color === 'blue');
                                        const redScore = red.score;
                                        const blueScore = blue.score;
                                        const redWon = redScore > blueScore;
                                        const blueWon = blueScore > redScore;
                                        const tie = redScore === blueScore;

                                        const isRed = red.teams.some(rt => rt.team.id === teamId);
                                        const won = isRed ? redWon : blueWon;
                                        const lost = isRed ? blueWon : redWon;

                                        chunkMap[teamId].matches++;
                                        if (won) chunkMap[teamId].wins++;
                                        if (lost) chunkMap[teamId].losses++;
                                        if (tie) chunkMap[teamId].ties++;

                                        let wp = 0;
                                        if (won) wp += 2;
                                        if (tie) wp += 1;
                                        if ((isRed ? red : blue).autonomous_wp) wp += 1;

                                        chunkMap[teamId].wp += wp;
                                    });
                                }
                            } catch (err) {
                                // console.log(`Failed to fetch matches for team ${teamId}`, err);
                            }
                        };

                        await Promise.all(chunk.map(id => fetchTeamMatches(id)));

                        // Merge into full map
                        fullRankingsMap = { ...fullRankingsMap, ...chunkMap };

                        // Update local state incrementally so user sees progress
                        setAllSeasonRankings(prev => ({ ...prev, ...chunkMap }));

                        // Safety Delay
                        await new Promise(r => setTimeout(r, 1200));
                    }
                }
            } catch (e) {
                console.error("Failed to fetch global season stats:", e);
            }
            return fullRankingsMap;
        };

        // Create the promise and store it
        const fetchPromise = fetchGlobalSeasonStats().then(data => {
            SEASON_STATS_CACHE[cacheKey] = data; // Cache result
            delete SEASON_STATS_PROMISES[cacheKey]; // Cleanup promise
            return data;
        });

        SEASON_STATS_PROMISES[cacheKey] = fetchPromise;

    }, [event?.id, division?.id, selectedSeason?.id]);

    useEffect(() => {
        fetchSeasonData();
    }, [fetchSeasonData]);

    const onRefresh = useCallback(() => {
        setRefreshing(true);
        fetchSeasonData();
    }, [fetchSeasonData]);

    const tabs = [
        { id: 'info', label: 'Info' },
        ...(event ? [
            { id: 'matches', label: 'Matches' },
            { id: 'stats', label: 'Stats' }
        ] : [
            { id: 'events', label: 'Events' }
        ]),
    ];

    return (
        <View style={styles.viewContainer}>
            <View style={{ flexDirection: 'row', backgroundColor: THEME.colors.background, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                {tabs.map(tab => (
                    <ScaleButton
                        key={tab.id}
                        onPress={() => {
                            if (activeTab !== tab.id) {
                                Animated.sequence([
                                    Animated.parallel([
                                        Animated.timing(fadeAnim, {
                                            toValue: 0,
                                            duration: 150,
                                            useNativeDriver: true,
                                        }),
                                        Animated.timing(slideAnim, {
                                            toValue: tab.id === 'info' ? -30 : 30,
                                            duration: 150,
                                            useNativeDriver: true,
                                        }),
                                    ]),
                                ]).start(() => {
                                    setActiveTab(tab.id);
                                    slideAnim.setValue(tab.id === 'info' ? 30 : -30);
                                    Animated.parallel([
                                        Animated.timing(fadeAnim, {
                                            toValue: 1,
                                            duration: 200,
                                            useNativeDriver: true,
                                        }),
                                        Animated.timing(slideAnim, {
                                            toValue: 0,
                                            duration: 200,
                                            useNativeDriver: true,
                                        }),
                                    ]).start();
                                });
                            }
                        }}
                        style={{
                            flex: 1,
                            paddingVertical: 12,
                            alignItems: 'center',
                            borderBottomWidth: 2,
                            borderBottomColor: activeTab === tab.id ? THEME.colors.primary : 'transparent'
                        }}
                        activeScale={0.9}
                    >
                        <Text style={{
                            color: activeTab === tab.id ? THEME.colors.primary : THEME.colors.text.secondary,
                            fontWeight: 'bold',
                            textTransform: 'uppercase'
                        }}>{tab.label}</Text>
                    </ScaleButton>
                ))}
            </View>

            <Animated.View style={{
                flex: 1,
                opacity: fadeAnim,
                transform: [{ translateX: slideAnim }],
                display: activeTab === 'info' ? 'flex' : 'none',
                backgroundColor: THEME.colors.background // Ensure opaque background
            }}>
                {loading && <LoadingScreen progress={0.5} message="Loading Team Data..." />}
                <ScrollView contentContainerStyle={{ padding: 16, paddingBottom: 100 }} refreshControl={<RefreshControl refreshing={refreshing} onRefresh={onRefresh} tintColor={THEME.colors.primary} />}>
                    <View style={{ alignItems: 'center', padding: 24, width: '100%' }}>
                        <View style={{ position: 'absolute', right: 0, top: 0, flexDirection: 'row' }}>
                            <ScaleButton onPress={() => openRobotEvents('team', team)} style={{ padding: 12 }}>
                                <View pointerEvents="none">
                                    <ExternalLink color={THEME.colors.primary} size={28} />
                                </View>
                            </ScaleButton>
                            <ScaleButton onPress={() => onToggleFavorite(team, 'team')} style={{ padding: 12 }}>
                                <View pointerEvents="none">
                                    <Star color={isFavorite ? THEME.colors.primary : THEME.colors.text.secondary} fill={isFavorite ? THEME.colors.primary : 'none'} size={28} />
                                </View>
                            </ScaleButton>
                        </View>
                        <Text style={{ fontSize: 48, fontWeight: '900', color: THEME.colors.primary, letterSpacing: -1 }}>{team.number}</Text>
                        {meowScore !== null && (
                            <View style={{ marginTop: 12, alignItems: 'center', gap: 8 }}>
                                <View style={{
                                    paddingHorizontal: 16,
                                    paddingVertical: 8,
                                    backgroundColor: getMeowRank(meowScore).color + '20',
                                    borderRadius: 12,
                                    borderWidth: 2,
                                    borderColor: getMeowRank(meowScore).color,
                                    flexDirection: 'row',
                                    alignItems: 'center',
                                    gap: 8
                                }}>
                                    <Text style={{ fontSize: 20 }}>{getMeowRank(meowScore).emoji}</Text>
                                    <Text style={{
                                        fontSize: 16,
                                        fontWeight: 'bold',
                                        color: getMeowRank(meowScore).color,
                                        textTransform: 'uppercase',
                                        letterSpacing: 1
                                    }}>
                                        {getMeowRank(meowScore).name}
                                    </Text>
                                </View>
                                <Text style={{
                                    fontSize: 13,
                                    color: THEME.colors.text.secondary,
                                    fontWeight: '500'
                                }}>
                                    MeowScore�? {meowScore}
                                </Text>
                            </View>
                        )}
                        <Text
                            style={{ fontSize: 24, fontWeight: 'bold', marginTop: 16, textAlign: 'center', width: '100%', paddingHorizontal: 16, color: THEME.colors.text.primary, textTransform: 'uppercase' }}
                            numberOfLines={3}
                            ellipsizeMode="tail"
                        >
                            {team.team_name}
                        </Text>
                        <Text
                            style={{ color: THEME.colors.text.secondary, marginTop: 8, textAlign: 'center', width: '100%', paddingHorizontal: 16 }}
                            numberOfLines={2}
                            ellipsizeMode="tail"
                        >
                            {team.organization}
                        </Text>
                        <Text style={{ color: THEME.colors.text.secondary, textAlign: 'center', width: '100%', paddingHorizontal: 16 }}>
                            {team.location?.city}, {team.location?.region}
                        </Text>
                        {team.location?.country && (
                            <Text style={{ color: THEME.colors.text.secondary, textAlign: 'center', width: '100%', paddingHorizontal: 16 }}>{team.location.country}</Text>
                        )}
                        {event && (
                            <View style={{ marginTop: 16, padding: 12, backgroundColor: THEME.colors.surface, borderRadius: 12, width: '100%', alignItems: 'center' }}>
                                <Text style={{ fontSize: 12, color: THEME.colors.text.secondary, textTransform: 'uppercase', fontWeight: 'bold', marginBottom: 4 }}>Viewing from Event</Text>
                                <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.primary, textAlign: 'center' }}>{event.name}</Text>
                                {eventRanking && (
                                    <View style={{ flexDirection: 'row', gap: 16, marginTop: 8 }}>
                                        <Text style={{ fontSize: 14, color: THEME.colors.text.primary }}>Rank: <Text style={{ fontWeight: 'bold' }}>{eventRanking.rank}</Text></Text>
                                        <Text style={{ fontSize: 14, color: THEME.colors.text.primary }}>W-L-T: <Text style={{ fontWeight: 'bold' }}>{eventRanking.wins}-{eventRanking.losses}-{eventRanking.ties}</Text></Text>
                                    </View>
                                )}
                            </View>
                        )}
                    </View>

                    <View style={[styles.card, { flexDirection: 'column', alignItems: 'stretch' }]}>
                        <View style={styles.formItem}>
                            <Text style={styles.label}>Program</Text>
                            <Text style={styles.input}>{team.program?.name || 'N/A'}</Text>
                        </View>
                        <View style={styles.formItem}>
                            <Text style={styles.label}>Grade</Text>
                            <Text style={styles.input}>{team.grade || 'N/A'}</Text>
                        </View>
                        {team.robot_name && (
                            <View style={styles.formItem}>
                                <Text style={styles.label}>Robot Name</Text>
                                <Text style={styles.input}>{team.robot_name}</Text>
                            </View>
                        )}
                    </View>

                    {selectedSeason && (
                        <View style={[styles.card, { flexDirection: 'column', alignItems: 'stretch' }]}>
                            <Text
                                style={{ fontSize: 16, fontWeight: 'bold', marginBottom: 12, color: THEME.colors.text.primary }}
                                numberOfLines={2}
                                ellipsizeMode="tail"
                            >
                                {selectedSeason.name} Season Stats
                            </Text>
                            {loading ? (
                                <ActivityIndicator size="small" color={THEME.colors.primary} />
                            ) : (
                                <View style={{ flexDirection: 'column', gap: 0 }}>
                                    <View style={{ padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                                        <View style={{ flex: 1 }}>
                                            <Text style={{ fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 }}>
                                                {seasonStats.isVexIQ ? 'Avg Score' : 'Record (W-L-T)'}
                                            </Text>
                                            <Text style={{ fontSize: 14, color: THEME.colors.text.primary }}>
                                                {seasonStats.isVexIQ
                                                    ? seasonStats.avgScore
                                                    : `${seasonStats.wins}-${seasonStats.losses}-${seasonStats.ties}`
                                                }
                                            </Text>
                                        </View>
                                    </View>
                                    <ScaleButton
                                        style={{ padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}
                                        onPress={() => pushView('teamAwards', { team, awards, season: selectedSeason })}
                                    >
                                        <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' }}>
                                            <View style={{ flex: 1 }}>
                                                <Text style={{ fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 }}>
                                                    Awards
                                                </Text>
                                                <Text style={{ fontSize: 14, color: THEME.colors.text.primary }}>{awards.length} total</Text>
                                            </View>
                                            <ChevronRight color={THEME.colors.text.secondary} size={20} />
                                        </View>
                                    </ScaleButton>
                                </View>
                            )}
                        </View>
                    )}

                    {skills && (
                        <View style={[styles.card, { flexDirection: 'column', alignItems: 'stretch' }]}>
                            <Text
                                style={{ fontSize: 16, fontWeight: 'bold', marginBottom: 12, color: THEME.colors.text.primary }}
                            >
                                World Skills Standings
                            </Text>
                            <View style={{ flexDirection: 'column', gap: 0 }}>
                                <View style={{ padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                                    <View style={{ flex: 1, flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' }}>
                                        <View>
                                            <Text style={{ fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 }}>
                                                Combined Score
                                            </Text>
                                            <Text style={{ fontSize: 28, fontWeight: '900', color: THEME.colors.primary }}>
                                                {skills.score}
                                            </Text>
                                        </View>
                                        {skills.rank && (
                                            <View style={{ alignItems: 'flex-end' }}>
                                                <Text style={{ fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 }}>
                                                    Global Rank
                                                </Text>
                                                <View style={{ backgroundColor: THEME.colors.primary, paddingHorizontal: 10, paddingVertical: 4, borderRadius: 8 }}>
                                                    <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.inverse }}>
                                                        #{skills.rank}
                                                    </Text>
                                                </View>
                                            </View>
                                        )}
                                    </View>
                                </View>
                                <View style={{ padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                                    <View style={{ flex: 1 }}>
                                        <Text style={{ fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 }}>
                                            Driver Score
                                        </Text>
                                        <Text style={{ fontSize: 14, color: THEME.colors.text.primary }}>
                                            {skills.driver}
                                        </Text>
                                    </View>
                                </View>
                                <View style={{ padding: 16 }}>
                                    <View style={{ flex: 1 }}>
                                        <Text style={{ fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 }}>
                                            Programming Score
                                        </Text>
                                        <Text style={{ fontSize: 14, color: THEME.colors.text.primary }}>
                                            {skills.programming}
                                        </Text>
                                    </View>
                                </View>
                            </View>
                        </View>
                    )}
                </ScrollView>
            </Animated.View>

            <Animated.View style={{
                flex: 1,
                opacity: fadeAnim,
                transform: [{ translateX: slideAnim }],
                display: activeTab === 'matches' ? 'flex' : 'none'
            }}>
                <View style={{ padding: 16, paddingBottom: 0, flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' }}>
                    <View>
                        <Text style={{ fontSize: 32, fontWeight: '900', color: THEME.colors.primary, letterSpacing: -1 }}>{team.number}</Text>
                        <Text style={{ color: THEME.colors.text.secondary, fontSize: 14 }}>{team.location?.city}, {team.location?.region}</Text>
                    </View>
                    <ScaleButton
                        style={{ flexDirection: 'row', alignItems: 'center', backgroundColor: simulate ? THEME.colors.primary : THEME.colors.surface, paddingHorizontal: 12, paddingVertical: 8, borderRadius: 20, gap: 6, borderWidth: 1, borderColor: simulate ? THEME.colors.primary : THEME.colors.border }}
                        onPress={() => setSimulate(!simulate)}
                    >
                        <Swords size={16} color={simulate ? THEME.colors.text.inverse : THEME.colors.text.primary} />
                        <Text style={{ fontSize: 12, fontWeight: 'bold', color: simulate ? THEME.colors.text.inverse : THEME.colors.text.primary }}>
                            {simulate ? 'Simulating' : 'Simulate'}
                        </Text>
                    </ScaleButton>
                </View>
                <FlatList
                    data={eventMatches}
                    keyExtractor={item => item.id.toString()}
                    contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                    renderItem={({ item }) => {
                        const redAlliance = item.alliances?.find(a => a.color === 'red');
                        const blueAlliance = item.alliances?.find(a => a.color === 'blue');

                        if (!redAlliance || !blueAlliance) return null;

                        const isRed = redAlliance.teams?.some(t => t.team.id === team.id);

                        const redScore = redAlliance?.score || 0;
                        const blueScore = blueAlliance?.score || 0;
                        const redWon = redScore > blueScore;
                        const blueWon = blueScore > redScore;
                        const tied = redScore === blueScore;

                        // Determine match result for this team
                        const isPlayed = item.scored || item.started || (redScore + blueScore !== 0);
                        let matchResult = 'default';

                        if (!isPlayed) {
                            matchResult = 'unplayed';
                        } else if (isRed) {
                            matchResult = redWon ? 'win' : (tied ? 'tie' : 'loss');
                        } else {
                            matchResult = blueWon ? 'win' : (tied ? 'tie' : 'loss');
                        }

                        const matchColor = matchResult === 'win' ? '#4ADE80' : // Green
                            matchResult === 'loss' ? THEME.colors.error : // Red
                                matchResult === 'tie' ? '#FACC15' : // Yellow
                                    THEME.colors.text.primary;

                        // Format time
                        const matchTime = item.scheduled ? new Date(item.scheduled).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', hour12: false }) : '';

                        const isVexIQ = team.program?.id === 41 || team.program?.code === 'VIQC';

                        if (isVexIQ) {
                            const score = redAlliance?.score || blueAlliance?.score || 0;
                            const teams = [...(redAlliance?.teams || []), ...(blueAlliance?.teams || [])];

                            let matchName = item.name;
                            matchName = matchName
                                .replace(/Practice\s?#?/, 'P')
                                .replace(/Teamwork\s*(Match)?\s*#?/i, 'Q')
                                .replace(/Match\s?#?/, 'F')
                                .replace(/Qualifier\s?#?/, 'Q')
                                .replace(/#/, '');

                            return (
                                <View style={[styles.listItem, { paddingVertical: 8 }]}>
                                    <View style={{ flexDirection: 'row', alignItems: 'center', flex: 1 }}>
                                        {/* Column 1: Name & Time */}
                                        <View style={{ width: 50, marginRight: 8 }}>
                                            <Text style={{ fontWeight: 'bold', color: THEME.colors.text.primary, fontSize: 14 }}>{matchName}</Text>
                                            {matchTime ? <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>{matchTime}</Text> : null}
                                        </View>

                                        {/* Column 2: Teams */}
                                        <View style={{ flex: 1, flexDirection: 'row', flexWrap: 'wrap', gap: 8 }}>
                                            {teams.map(t => (
                                                <Text key={t.team.id} style={{
                                                    color: THEME.colors.blueAlliance,
                                                    fontSize: 14,
                                                    fontWeight: t.team.id === team.id ? 'bold' : 'normal'
                                                }}>
                                                    {t.team.name}
                                                </Text>
                                            ))}
                                        </View>



                                        {/* Column 3: Score */}
                                        <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.text.primary, width: 40, textAlign: 'right' }}>
                                            {(item.scored || item.started || score > 0) ? score : ''}
                                        </Text>
                                    </View>
                                </View>
                            );
                        }

                        let matchName = item.name;
                        matchName = matchName
                            .replace(/Practice\s?#?/, 'P')
                            .replace(/Teamwork\s*(Match)?\s*#?/i, 'Q')
                            .replace(/Qualification\s?#?/, 'Q')
                            .replace(/Qualifier\s?#?/, 'Q')
                            .replace(/Quarterfinal\s?#?/, 'QF')
                            .replace(/Semifinal\s?#?/, 'SF')
                            .replace(/Final\s?#?/, 'F')
                            .replace(/Round of 16\s?#?/, 'R16')
                            .replace(/#/, '');

                        return (
                            <ScaleButton
                                key={item.id}
                                onPress={() => pushView('matchDetail', item)}
                                style={[styles.listItem, { paddingVertical: 8, flexDirection: 'column', alignItems: 'stretch' }]}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', flex: 1 }}>
                                    {/* Column 1: Name & Time */}
                                    <View style={{ width: 65 }}>
                                        <Text style={{ fontWeight: 'bold', color: matchColor, fontSize: 14 }}>{matchName}</Text>
                                        {matchTime ? <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>{matchTime}</Text> : null}
                                    </View>

                                    {/* Center Block */}
                                    <View style={{ flex: 1, flexDirection: 'row', justifyContent: 'center', alignItems: 'center' }}>
                                        {/* Red Teams */}
                                        <View style={{ width: 60, alignItems: 'flex-end', marginRight: 8 }}>
                                            {redAlliance?.teams?.map(t => (
                                                <Text key={t.team.id} style={{
                                                    color: THEME.colors.error,
                                                    fontSize: 12,
                                                    fontWeight: t.team.id === team.id ? 'bold' : 'normal'
                                                }}>
                                                    {t.team.name}
                                                </Text>
                                            ))}
                                        </View>

                                        {/* Red Score */}
                                        <Text style={{
                                            color: THEME.colors.error,
                                            fontWeight: 'bold',
                                            fontSize: 16,
                                            width: 35,
                                            textAlign: 'center',
                                            textDecorationLine: redWon ? 'underline' : 'none',
                                        }}>
                                            {(item.scored || item.started || (redScore + blueScore !== 0)) ? redScore : ''}
                                        </Text>

                                        {/* Score Spacer */}
                                        <View style={{ width: 24 }} />

                                        {/* Blue Score */}
                                        <Text style={{
                                            color: THEME.colors.blueAlliance,
                                            fontWeight: 'bold',
                                            fontSize: 16,
                                            width: 35,
                                            textAlign: 'center',
                                            textDecorationLine: blueWon ? 'underline' : 'none',
                                        }}>
                                            {(item.scored || item.started || (redScore + blueScore !== 0)) ? blueScore : ''}
                                        </Text>

                                        {/* Blue Teams */}
                                        <View style={{ width: 60, alignItems: 'flex-start', marginLeft: 8 }}>
                                            {blueAlliance?.teams?.map(t => (
                                                <Text key={t.team.id} style={{
                                                    color: THEME.colors.blueAlliance,
                                                    fontSize: 12,
                                                    fontWeight: t.team.id === team.id ? 'bold' : 'normal'
                                                }}>
                                                    {t.team.name}
                                                </Text>
                                            ))}
                                        </View>
                                    </View>

                                    <ChevronRight size={16} color={THEME.colors.text.secondary} />
                                </View>

                                {/* Simulation Details */}
                                {simulate && (
                                    <View style={{ width: '100%', marginTop: 8, padding: 8, backgroundColor: THEME.colors.background, borderRadius: 8 }}>
                                        {(() => {
                                            const getHybridOpr = (tId) => {
                                                const eventStat = allEventStats[tId];
                                                const eventOpr = eventStat ? eventStat.opr : 0;
                                                const matchesPlayed = eventStat ? eventStat.matchesPlayed : 0;

                                                // Calculate Season MeowScore (0-100) for Global Estimate
                                                const ranking = allSeasonRankings[tId];

                                                let globalOpr = 15; // Default fallback average

                                                if (ranking) {
                                                    const totalGames = ranking.wins + ranking.losses + ranking.ties;
                                                    const winPct = totalGames > 0 ? (ranking.wins / totalGames) * 100 : 0;

                                                    // Season MeowScore Components (Lite Version)
                                                    // 1. Win Performance (50% weight) - Strong indicator of season success
                                                    const winScore = winPct * 0.5;

                                                    // 2. AP/WP Efficiency (50% weight)
                                                    // Max WP per match is ~6. Normalize to 100.
                                                    const wpPerMatch = totalGames > 0 ? ranking.wp / totalGames : 0;
                                                    const wpScore = Math.min((wpPerMatch / 4) * 100, 100) * 0.5;

                                                    // Note: Skills is EXCLUDED as per request.

                                                    const seasonMeowScore = winScore + wpScore;

                                                    // Convert MeowScore (0-100) to OPR points
                                                    // Scaling: 100 Meow ~= 35 OPR
                                                    globalOpr = (seasonMeowScore * 0.35) + 5;
                                                }

                                                // Hybrid Weighting
                                                // Tournament Performance = Major Reference
                                                // Season MeowScore = Minor Reference
                                                const statsWeight = matchesPlayed * 2; // Increases rapidly with matches
                                                const globalWeight = 2; // Constant small anchor

                                                const hybridOpr = ((eventOpr * statsWeight) + (globalOpr * globalWeight)) / (statsWeight + globalWeight);
                                                return hybridOpr;
                                            };

                                            const redOpr = redAlliance.teams.reduce((acc, t) => acc + getHybridOpr(t.team.id), 0);
                                            const blueOpr = blueAlliance.teams.reduce((acc, t) => acc + getHybridOpr(t.team.id), 0);

                                            const redProb = calculateWinProbability(redOpr - blueOpr);
                                            const blueProb = 100 - redProb;

                                            const predictionColor = redProb > 50 ? THEME.colors.error : THEME.colors.blueAlliance;
                                            const winProb = redProb > 50 ? redProb : blueProb;
                                            const winnerName = redProb > 50 ? 'Red' : 'Blue';

                                            return (
                                                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between' }}>
                                                    <View style={{ flexDirection: 'row', alignItems: 'center', gap: 8 }}>
                                                        <View style={{ backgroundColor: THEME.colors.surface, paddingHorizontal: 6, paddingVertical: 2, borderRadius: 4 }}>
                                                            <Text style={{ color: THEME.colors.error, fontWeight: 'bold' }}>{redOpr.toFixed(0)}</Text>
                                                        </View>
                                                        <Text style={{ color: THEME.colors.text.secondary, fontSize: 10 }}>vs</Text>
                                                        <View style={{ backgroundColor: THEME.colors.surface, paddingHorizontal: 6, paddingVertical: 2, borderRadius: 4 }}>
                                                            <Text style={{ color: THEME.colors.blueAlliance, fontWeight: 'bold' }}>{blueOpr.toFixed(0)}</Text>
                                                        </View>
                                                    </View>

                                                    <View style={{ flexDirection: 'row', alignItems: 'center', gap: 6 }}>
                                                        <Text style={{ color: THEME.colors.text.secondary, fontSize: 10 }}>Win %</Text>
                                                        <View style={{ width: 60, height: 6, backgroundColor: THEME.colors.surface, borderRadius: 3, overflow: 'hidden' }}>
                                                            <View style={{ width: `${winProb}%`, height: '100%', backgroundColor: predictionColor }} />
                                                        </View>
                                                        <Text style={{ color: predictionColor, fontWeight: 'bold', fontSize: 12 }}>{winProb}%</Text>
                                                    </View>
                                                </View>
                                            );
                                        })()}
                                    </View>
                                )}
                            </ScaleButton>
                        );
                    }}
                />
            </Animated.View>

            <Animated.View style={{
                flex: 1,
                opacity: fadeAnim,
                transform: [{ translateX: slideAnim }],
                display: activeTab === 'stats' ? 'flex' : 'none'
            }}>
                <ScrollView contentContainerStyle={{ padding: 16, paddingBottom: 100 }}>
                    {/* Header */}
                    <View style={{ alignItems: 'center', marginBottom: 24, marginTop: 8 }}>
                        <Text style={{ fontSize: 36, fontWeight: '900', color: '#CCFF00', letterSpacing: -1 }}>{team.number}</Text>
                        <Text style={{ color: '#8e8e93', fontSize: 14 }}>{team.location?.city}, {team.location?.region}</Text>
                    </View>

                    {/* Current Rank Card */}
                    {eventRanking && (
                        <View style={{
                            backgroundColor: '#1c1c1e',
                            borderRadius: 16,
                            padding: 20,
                            marginBottom: 16,
                            borderWidth: 1,
                            borderColor: '#2c2c2e'
                        }}>
                            <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', marginBottom: 8, textTransform: 'uppercase' }}>Current Rank</Text>
                            <Text style={{ fontSize: 48, fontWeight: '900', color: '#CCFF00' }}>#{eventRanking.rank}</Text>
                            {/* Optional Trophy Icon could go here absolutely positioned */}
                        </View>
                    )}

                    {/* Stats Grid */}
                    <View style={{ flexDirection: 'row', flexWrap: 'wrap', gap: 12 }}>
                        {/* Record - Hide for VEX IQ */}
                        {!isVexIQ && (
                            <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                    <Hash size={14} color="#8e8e93" />
                                    <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>Record</Text>
                                </View>
                                <Text style={{ fontSize: 24, fontWeight: 'bold', color: '#ffffff' }}>
                                    {eventRanking ? `${eventRanking.wins}-${eventRanking.losses}-${eventRanking.ties}` : '-'}
                                </Text>
                            </View>
                        )}

                        {/* WP / AP / SP - Hide for VEX IQ */}
                        {!isVexIQ && (
                            <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                    <Target size={14} color="#CCFF00" />
                                    <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>WP / AP / SP</Text>
                                </View>
                                <Text style={{ fontSize: 20, fontWeight: 'bold', color: '#ffffff' }}>
                                    {eventRanking ? `${eventRanking.wp} / ${eventRanking.ap} / ${eventRanking.sp}` : '-'}
                                </Text>
                            </View>
                        )}

                        {/* High Score */}
                        <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                            <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                <TrendingUp size={14} color="#8e8e93" />
                                <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>High Score</Text>
                            </View>
                            <Text style={{ fontSize: 24, fontWeight: 'bold', color: '#ffffff' }}>
                                {(() => {
                                    if (!eventMatches || eventMatches.length === 0) return '-';
                                    let max = 0;
                                    eventMatches.forEach(m => {
                                        const alliance = m.alliances.find(a => a.teams.some(t => t.team.id === team.id));
                                        if (alliance && alliance.score > max) max = alliance.score;
                                    });
                                    return max;
                                })()}
                            </Text>
                        </View>

                        {/* Average Score - Show for VEX IQ */}
                        {isVexIQ && (
                            <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                    <TrendingUp size={14} color="#8e8e93" />
                                    <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>Avg Score</Text>
                                </View>
                                <Text style={{ fontSize: 24, fontWeight: 'bold', color: '#ffffff' }}>
                                    {(() => {
                                        if (!eventMatches || eventMatches.length === 0) return '-';
                                        let total = 0;
                                        let count = 0;
                                        eventMatches.forEach(m => {
                                            const alliance = m.alliances.find(a => a.teams.some(t => t.team.id === team.id));
                                            if (alliance) {
                                                total += alliance.score;
                                                count++;
                                            }
                                        });
                                        return count > 0 ? (total / count).toFixed(1) : '-';
                                    })()}
                                </Text>
                            </View>
                        )}

                        {/* OPR - Hide for VEX IQ */}
                        {!isVexIQ && (
                            <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                    <Zap size={14} color="#ef4444" />
                                    <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>OPR</Text>
                                </View>
                                <Text style={{ fontSize: 24, fontWeight: 'bold', color: '#ffffff' }}>
                                    {eventStats.opr?.toFixed(2) || '-'}
                                </Text>
                            </View>
                        )}

                        {/* DPR - Hide for VEX IQ */}
                        {!isVexIQ && (
                            <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                    <Shield size={14} color="#3b82f6" />
                                    <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>DPR</Text>
                                </View>
                                <Text style={{ fontSize: 24, fontWeight: 'bold', color: '#ffffff' }}>
                                    {eventStats.dpr?.toFixed(2) || '-'}
                                </Text>
                            </View>
                        )}

                        {/* CCWM - Hide for VEX IQ */}
                        {!isVexIQ && (
                            <View style={{ width: '48%', backgroundColor: '#1c1c1e', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2c2c2e' }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginBottom: 8, gap: 6 }}>
                                    <Award size={14} color="#CCFF00" />
                                    <Text style={{ color: '#8e8e93', fontSize: 12, fontWeight: 'bold', textTransform: 'uppercase' }}>CCWM</Text>
                                </View>
                                <Text style={{ fontSize: 24, fontWeight: 'bold', color: '#ffffff' }}>
                                    {eventStats.ccwm?.toFixed(2) || '-'}
                                </Text>
                            </View>
                        )}
                    </View>
                </ScrollView>
            </Animated.View>

            <Animated.View style={{
                flex: 1,
                opacity: fadeAnim,
                transform: [{ translateX: slideAnim }],
                display: activeTab === 'events' ? 'flex' : 'none'
            }}>
                <FlatList
                    data={events}
                    keyExtractor={item => item.id.toString()}
                    contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                    refreshControl={<RefreshControl refreshing={refreshing} onRefresh={onRefresh} />}
                    renderItem={({ item }) => (
                        <ScaleButton
                            style={styles.card}
                            onPress={() => pushView('eventDetail', item)}
                        >
                            <View style={{ flex: 1 }}>
                                <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.text.primary }}>{item.name}</Text>
                                <View style={{ flexDirection: 'row', alignItems: 'center', marginTop: 4 }}>
                                    <Calendar size={14} color={THEME.colors.text.secondary} />
                                    <Text style={{ color: THEME.colors.text.secondary, marginLeft: 4, fontSize: 12 }}>
                                        {new Date(item.start).toLocaleDateString()}
                                    </Text>
                                    <MapPin size={14} color={THEME.colors.text.secondary} style={{ marginLeft: 12 }} />
                                    <Text style={{ color: 'gray', marginLeft: 4, fontSize: 12 }}>
                                        {item.location?.city}, {item.location?.region}
                                    </Text>
                                </View>
                            </View>
                            <ChevronRight color="#9ca3af" size={20} />
                        </ScaleButton>
                    )}
                    ListEmptyComponent={
                        <View style={{ padding: 20, alignItems: 'center' }}>
                            <Text style={{ color: 'gray' }}>No events found for this season.</Text>
                        </View>
                    }
                />
            </Animated.View>
        </View>
    );
});

// --- Team Awards View ---

const TeamAwardsView = React.memo(({ team, awards, season }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    return (
        <View style={styles.viewContainer}>
            <FlatList
                data={awards}
                keyExtractor={(item, index) => `${item.id}-${index}`}
                contentContainerStyle={{ padding: 16 }}
                ListHeaderComponent={() => (
                    <View style={{ marginBottom: 16 }}>
                        <Text style={{ fontSize: 20, fontWeight: 'bold', marginBottom: 4, color: THEME.colors.text.primary }}>{team.team_name}</Text>
                        <Text style={{ fontSize: 14, color: THEME.colors.text.secondary }}>{season.name} Awards ({awards.length})</Text>
                    </View>
                )}
                renderItem={({ item }) => (
                    <View style={styles.listItem}>
                        <View style={{ flex: 1 }}>
                            <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.primary }}>{item.title}</Text>
                            {item.event && (
                                <Text style={{ fontSize: 12, color: THEME.colors.text.secondary, marginTop: 4 }}>{item.event.name}</Text>
                            )}
                            {item.qualifications && item.qualifications.length > 0 && (
                                <Text style={{ fontSize: 12, color: THEME.colors.text.secondary, marginTop: 4 }}>
                                    {item.qualifications.join(', ')}
                                </Text>
                            )}
                        </View>
                    </View>
                )}
                ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No awards found.</Text>}
            />
        </View>
    );
});

const MatchDetailView = React.memo(({ match, pushView }) => {
    const { theme: THEME } = useTheme();
    console.log('MATCH DATA:', JSON.stringify(match, null, 2));

    const redAlliance = match.alliances?.find(a => a.color === 'red');
    const blueAlliance = match.alliances?.find(a => a.color === 'blue');

    const redScore = redAlliance?.score || 0;
    const blueScore = blueAlliance?.score || 0;

    const redWon = redScore > blueScore;
    const blueWon = blueScore > redScore;

    const isPlayed = match.scored || match.started || (redScore + blueScore > 0);

    const matchTime = match.scheduled ? new Date(match.scheduled).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }) : 'TBD';
    const matchName = match.name;

    return (
        <ScrollView style={{ flex: 1, backgroundColor: THEME.colors.background }} contentContainerStyle={{ padding: 16 }}>
            <View style={{
                backgroundColor: THEME.colors.surface,
                borderRadius: 24,
                padding: 24,
                marginBottom: 24,
                alignItems: 'center',
                shadowColor: '#000',
                shadowOffset: { width: 0, height: 2 },
                shadowOpacity: 0.1,
                shadowRadius: 8,
                elevation: 3
            }}>
                <Text style={{ fontSize: 24, fontWeight: '900', color: THEME.colors.text.primary, marginBottom: 8 }}>{matchName}</Text>
                <View style={{ flexDirection: 'row', alignItems: 'center', gap: 6, marginBottom: 24 }}>
                    <Clock size={16} color={THEME.colors.text.secondary} />
                    <Text style={{ fontSize: 16, color: THEME.colors.text.secondary }}>{matchTime}</Text>
                </View>

                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'center', gap: 32, width: '100%' }}>
                    <View style={{ alignItems: 'center', justifyContent: 'center' }}>
                        <Text style={{ fontSize: 64, fontWeight: '900', color: isPlayed ? THEME.colors.error : THEME.colors.text.disabled, lineHeight: 70 }}>
                            {isPlayed ? redScore : '-'}
                        </Text>
                        <View style={{ height: 32, justifyContent: 'center' }}>
                            {redWon && isPlayed && <Trophy size={24} color={THEME.colors.error} />}
                        </View>
                    </View>

                    <Text style={{ fontSize: 32, fontWeight: 'bold', color: THEME.colors.text.secondary, paddingBottom: 32 }}>-</Text>

                    <View style={{ alignItems: 'center', justifyContent: 'center' }}>
                        <Text style={{ fontSize: 64, fontWeight: '900', color: isPlayed ? THEME.colors.blueAlliance : THEME.colors.text.disabled, lineHeight: 70 }}>
                            {isPlayed ? blueScore : '-'}
                        </Text>
                        <View style={{ height: 32, justifyContent: 'center' }}>
                            {blueWon && isPlayed && <Trophy size={24} color={THEME.colors.blueAlliance} />}
                        </View>
                    </View>
                </View>
            </View>

            {/* Detailed Statistics */}
            {isPlayed && (
                <View style={{ backgroundColor: THEME.colors.surface, borderRadius: 24, padding: 20, marginBottom: 24, gap: 16 }}>
                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: THEME.colors.text.primary, marginBottom: 8 }}>Match Stats</Text>

                    {/* Autonomous */}
                    <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', paddingVertical: 8, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                        <Text style={{ color: THEME.colors.text.secondary }}>Autonomous Winner</Text>
                        <Text style={{ fontWeight: 'bold', color: THEME.colors.text.primary, textTransform: 'capitalize' }}>
                            {/* Assuming 'auton_winner' field exists, otherwise N/A */}
                            {match.auton_winner ? match.auton_winner : 'Tie / None'}
                        </Text>
                    </View>

                    {/* Example Stats - will be populated purely based on visible props or common fields for now */}
                    {/* We can iterate on this once we see the log */}
                </View>
            )}

            <View style={{ flexDirection: 'row', gap: 16 }}>
                <View style={{ flex: 1, backgroundColor: 'rgba(239, 68, 68, 0.1)', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: THEME.colors.error }}>
                    <Text style={{ color: THEME.colors.error, fontWeight: 'bold', marginBottom: 16, textTransform: 'uppercase', fontSize: 12 }}>Red Alliance</Text>
                    <View style={{ gap: 12 }}>
                        {redAlliance?.teams?.map(t => (
                            <ScaleButton key={t.team.id} onPress={() => pushView('teamDetail', { team: t.team })}>
                                <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', backgroundColor: THEME.colors.surface, padding: 12, borderRadius: 12 }}>
                                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: THEME.colors.text.primary }}>{t.team.name}</Text>
                                    <ChevronRight size={16} color={THEME.colors.text.secondary} />
                                </View>
                            </ScaleButton>
                        ))}
                    </View>
                </View>

                <View style={{ flex: 1, backgroundColor: 'rgba(59, 130, 246, 0.1)', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: THEME.colors.blueAlliance }}>
                    <Text style={{ color: THEME.colors.blueAlliance, fontWeight: 'bold', marginBottom: 16, textTransform: 'uppercase', fontSize: 12 }}>Blue Alliance</Text>
                    <View style={{ gap: 12 }}>
                        {blueAlliance?.teams?.map(t => (
                            <ScaleButton key={t.team.id} onPress={() => pushView('teamDetail', { team: t.team })}>
                                <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', backgroundColor: THEME.colors.surface, padding: 12, borderRadius: 12 }}>
                                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: THEME.colors.text.primary }}>{t.team.name}</Text>
                                    <ChevronRight size={16} color={THEME.colors.text.secondary} />
                                </View>
                            </ScaleButton>
                        ))}
                    </View>
                </View>
            </View>
        </ScrollView>
    );
});

const DivisionDetailView = React.memo(({ event, division, apiKey, apiRequest, pushView }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const [activeTab, setActiveTab] = useState('teams');
    const [data, setData] = useState({ teams: [], matches: [], rankings: [], awards: [] });
    const [stats, setStats] = useState({});
    const [sortMethod, setSortMethod] = useState('number'); // number, rank, opr, dpr, ccwm
    const [loading, setLoading] = useState(false);

    const [meowScores, setMeowScores] = useState({});

    // Fetch data logic extended to ensure we have what we need for Meow Score
    // Fetch data logic extended to ensure we have what we need for Meow Score
    useEffect(() => {
        const fetchData = async (background = false) => {
            if (!apiKey) return;

            // If sorting by Meow Score, we need matches (for OPR) and rankings (for AP/WP/WLT)
            const needsDivMeowData = sortMethod === 'meowScore';

            if (!background) setLoading(true);
            try {
                const PER_PAGE = 250;

                // 1. Ensure Teams are fetched (if active tab is teams)
                if (activeTab === 'teams' && data.teams.length === 0) {
                    let endpoint = `/events/${event.id}/teams?per_page=250`;
                    // Fetch logic handled below generally, but ensuring we trigger it
                }

                // Generic fetch helper
                const fetchAllPages = async (endpoint) => {
                    let allItems = [];
                    const firstResponse = await apiRequest(endpoint, { page: 1, per_page: PER_PAGE });
                    if (firstResponse && Array.isArray(firstResponse.data)) {
                        allItems = [...firstResponse.data];
                        const lastPage = firstResponse.meta?.last_page || 1;
                        for (let p = 2; p <= lastPage; p++) {
                            const response = await apiRequest(endpoint, { page: p, per_page: PER_PAGE });
                            if (response && Array.isArray(response.data)) {
                                allItems = [...allItems, ...response.data];
                            }
                        }
                    }
                    return allItems;
                };

                // Fetch Teams logic with Division Filtering
                if (activeTab === 'teams' && data.teams.length === 0) {

                    // 1. Fetch Rankings and Matches to identify teams in this division
                    const [rankingsData, matchesData] = await Promise.all([
                        fetchAllPages(`/events/${event.id}/divisions/${division.id}/rankings?per_page=250`),
                        fetchAllPages(`/events/${event.id}/divisions/${division.id}/matches?per_page=250`)
                    ]);

                    const divisionTeamIds = new Set();

                    if (Array.isArray(rankingsData)) {
                        rankingsData.forEach(r => { if (r.team) divisionTeamIds.add(r.team.id); });
                    }

                    if (Array.isArray(matchesData)) {
                        matchesData.forEach(m => {
                            if (m.alliances) {
                                m.alliances.forEach(a => {
                                    if (a.teams) a.teams.forEach(t => { if (t.team) divisionTeamIds.add(t.team.id); });
                                });
                            }
                        });
                    }

                    // 2. Fetch ALL teams to get full details (location, name, etc.)
                    const allTeams = await fetchAllPages(`/events/${event.id}/teams?per_page=250`);

                    if (Array.isArray(allTeams)) {
                        // 3. Filter allTeams to only include those found in this division
                        // If no matches/rankings exist yet (pre-event), we might check if 'division' field exists on team object
                        // or fallback to showing nothing (better than showing all) or checks if allTeams has division data.

                        let filteredTeams = [];
                        if (divisionTeamIds.size > 0) {
                            filteredTeams = allTeams.filter(t => divisionTeamIds.has(t.id));
                        } else {
                            // Fallback: Check if team object has division info directly
                            filteredTeams = allTeams.filter(t => t.division && t.division.id === division.id);

                            // Double Fallback: If still empty and we have no division info, 
                            // it's safer to show NO teams than ALL teams for a specific division view.
                        }

                        setData(prev => ({ ...prev, teams: filteredTeams }));
                    }
                }

                // Fetch Schedule if needed (or for Meow Score)
                if ((activeTab === 'schedule' || needsDivMeowData) && (data.matches.length === 0 || background)) {
                    const matches = await fetchAllPages(`/events/${event.id}/divisions/${division.id}/matches?per_page=250`);

                    if (Array.isArray(matches)) {
                        // Sort matches
                        const sortMatches = (matches) => {
                            return matches.sort((a, b) => {
                                const getMatchPriority = (name) => {
                                    if (!name) return 999;
                                    const upper = name.toUpperCase();
                                    if (upper.startsWith('P')) return 0;
                                    if (upper.startsWith('Q')) {
                                        if (upper.includes('QUARTER') || upper.startsWith('QF')) return 3;
                                        return 1;
                                    }
                                    if (upper.startsWith('T')) return 1;
                                    if (upper.startsWith('R16') || upper.startsWith('R')) return 2;
                                    if (upper.startsWith('S')) return 4;
                                    if (upper.startsWith('F') || upper.startsWith('M')) return 5;
                                    return 6;
                                };
                                const getMatchNumber = (name) => {
                                    const match = name?.match(/\d+/);
                                    return match ? parseInt(match[0]) : 0;
                                };
                                const aPriority = getMatchPriority(a.name);
                                const bPriority = getMatchPriority(b.name);
                                if (aPriority !== bPriority) return aPriority - bPriority;
                                return getMatchNumber(a.name) - getMatchNumber(b.name);
                            });
                        };

                        setData(prev => ({ ...prev, matches: sortMatches(matches) }));
                    }
                }

                // Fetch Rankings if needed (or for Meow Score)
                if ((activeTab === 'rankings' || needsDivMeowData) && (data.rankings.length === 0 || background)) {
                    const rankings = await fetchAllPages(`/events/${event.id}/divisions/${division.id}/rankings?per_page=250`);
                    if (Array.isArray(rankings)) {
                        setData(prev => ({ ...prev, rankings }));
                    }
                }

                // Fetch Awards if needed
                if (activeTab === 'awards' && data.awards.length === 0) {
                    const awards = await fetchAllPages(`/events/${event.id}/awards?per_page=250`);
                    setData(prev => ({ ...prev, awards }));
                }

                // Calculate Stats & Meow Scores if we have data
                if ((activeTab === 'rankings' || needsDivMeowData) && (data.matches.length > 0 || needsDivMeowData)) {
                    // Check if we have what we need
                    const currentMatches = (data.matches.length > 0) ? data.matches : await fetchAllPages(`/events/${event.id}/divisions/${division.id}/matches?per_page=250`);
                    const currentRankings = (data.rankings.length > 0) ? data.rankings : await fetchAllPages(`/events/${event.id}/divisions/${division.id}/rankings?per_page=250`);

                    if (Array.isArray(currentMatches) && Array.isArray(currentRankings)) {
                        // Helper: Extract teams from rankings if teams not loaded, or use loaded teams
                        // Ideally we use the teams from rankings to ensure we only calc for this division's participants in ranking
                        const teamsForStats = currentRankings.map(r => r.team);

                        if (currentMatches.length > 0 && teamsForStats.length > 0) {
                            // 1. OPR/DPR/CCWM
                            const newStats = calculateStats(currentMatches, teamsForStats);
                            setStats(newStats);

                            // 2. Meow Scores
                            const scores = {};
                            currentRankings.forEach(rank => {
                                const teamId = rank.team.id;
                                const teamStats = {
                                    wins: rank.wins,
                                    losses: rank.losses,
                                    ties: rank.ties,
                                    ap: rank.ap,
                                    wp: rank.wp,
                                    opr: newStats[teamId]?.opr || 0,
                                    dpr: newStats[teamId]?.dpr || 0,
                                    ccwm: newStats[teamId]?.ccwm || 0,
                                    isVexIQ: event.program?.id === 41 || event.program?.code === 'VIQC'
                                };
                                scores[teamId] = calculateMeowScore(teamStats);
                            });
                            setMeowScores(scores);

                            // Ensure data state is updated if we fetched fresh
                            setData(prev => ({
                                ...prev,
                                matches: prev.matches.length === 0 ? currentMatches : prev.matches,
                                rankings: prev.rankings.length === 0 ? currentRankings : prev.rankings
                            }));
                        }
                    }
                }

            } catch (e) {
                console.error("Failed to fetch division details:", e);
            } finally {
                if (!background) setLoading(false);
            }
        };
        fetchData();

        // Live update interval (3 seconds)
        const intervalId = setInterval(() => {
            if (activeTab === 'schedule' || activeTab === 'rankings' || activeTab === 'teams') {
                // console.log("Auto-refreshing event data...");
                fetchData(true);
            }
        }, 3000);

        return () => clearInterval(intervalId);
    }, [activeTab, event.id, division.id, apiKey, apiRequest, sortMethod, data.teams.length]);

    const tabs = [
        { id: 'teams', label: 'Teams', icon: Users },
        { id: 'schedule', label: 'Schedule', icon: List },
        { id: 'rankings', label: 'Rankings', icon: Trophy },
        { id: 'awards', label: 'Awards', icon: Award },
    ];

    const sortedRankings = useMemo(() => {
        if (!data.rankings) return [];
        let sorted = [...data.rankings];
        if (sortMethod === 'rank' || sortMethod === 'number') {
            sorted.sort((a, b) => a.rank - b.rank);
        } else if (sortMethod === 'meowScore') {
            sorted.sort((a, b) => (meowScores[b.team.id] || 0) - (meowScores[a.team.id] || 0));
        } else {
            sorted.sort((a, b) => {
                const valA = stats[a.team.id]?.[sortMethod] || 0;
                const valB = stats[b.team.id]?.[sortMethod] || 0;
                return sortMethod === 'dpr' ? valA - valB : valB - valA;
            });
        }
        return sorted;
    }, [data.rankings, stats, sortMethod, meowScores]);

    const sortedTeams = useMemo(() => {
        if (!data.teams) return [];
        let sorted = [...data.teams];

        if (sortMethod === 'meowScore') {
            sorted.sort((a, b) => (meowScores[b.id] || 0) - (meowScores[a.id] || 0));
        } else if (sortMethod === 'rank') {
            sorted.sort((a, b) => {
                const rankA = data.rankings?.find(r => r.team.id === a.id)?.rank || 9999;
                const rankB = data.rankings?.find(r => r.team.id === b.id)?.rank || 9999;
                if (rankA !== rankB) return rankA - rankB;
                return parseInt(a.number) - parseInt(b.number);
            });
        } else {
            // Default to Number sort (handles 'number' and generic stats if needed, though stats have their own block usually)
            sorted.sort((a, b) => {
                const numA = parseInt(a.number.replace(/\D/g, ''));
                const numB = parseInt(b.number.replace(/\D/g, ''));
                if (numA !== numB) return numA - numB;
                return a.number.localeCompare(b.number);
            });
        }
        return sorted;
    }, [data.teams, data.rankings, meowScores, sortMethod]);

    const teamNameMap = useMemo(() => {
        const map = {};
        if (data.teams) {
            data.teams.forEach(t => { map[t.id] = t.team_name; });
        }
        return map;
    }, [data.teams]);

    const renderContent = () => {
        return (
            <View style={{ flex: 1 }}>
                {loading && <ActivityIndicator size="large" color={THEME.colors.primary} style={{ marginTop: 40 }} />}

                <View style={{ flex: 1, display: activeTab === 'teams' ? 'flex' : 'none' }}>
                    {/* Sort Controls */}
                    <View style={{ flexDirection: 'row', paddingHorizontal: 16, paddingTop: 16, gap: 10, flexWrap: 'wrap' }}>
                        <ScaleButton
                            onPress={() => setSortMethod('number')}
                            style={{
                                paddingHorizontal: 12, paddingVertical: 6, borderRadius: 20,
                                backgroundColor: sortMethod === 'number' ? THEME.colors.primary : THEME.colors.surface,
                                borderWidth: 1, borderColor: sortMethod === 'number' ? THEME.colors.primary : THEME.colors.border
                            }}
                        >
                            <Text style={{ color: sortMethod === 'number' ? 'black' : THEME.colors.text.secondary, fontWeight: 'bold', fontSize: 12 }}>Number</Text>
                        </ScaleButton>
                        <ScaleButton
                            onPress={() => setSortMethod('meowScore')}
                            style={{
                                paddingHorizontal: 12, paddingVertical: 6, borderRadius: 20,
                                backgroundColor: sortMethod === 'meowScore' ? THEME.colors.primary : THEME.colors.surface,
                                borderWidth: 1, borderColor: sortMethod === 'meowScore' ? THEME.colors.primary : THEME.colors.border
                            }}
                        >
                            <Text style={{ color: sortMethod === 'meowScore' ? 'black' : THEME.colors.text.secondary, fontWeight: 'bold', fontSize: 12 }}>Event MeowScore</Text>`r`n                        </ScaleButton>
                    </View>

                    <FlatList
                        data={sortedTeams}
                        keyExtractor={item => item.id.toString()}
                        contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                        renderItem={({ item, index }) => {
                            const score = meowScores[item.id];
                            const rank = score !== undefined ? getMeowRank(score) : null;
                            const showRank = sortMethod === 'meowScore' && rank;

                            return (
                                <AnimatedListItem index={index} delay={30}>
                                    <ScaleButton
                                        style={styles.listItem}
                                        onPress={() => pushView('teamDetail', { team: item, event, division })}
                                    >
                                        <View style={{ flex: 1 }}>
                                            <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between' }}>
                                                <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.primary }}>{item.number}</Text>
                                                {showRank && (
                                                    <View style={{ flexDirection: 'row', alignItems: 'center', backgroundColor: rank.color + '20', paddingHorizontal: 8, paddingVertical: 2, borderRadius: 12 }}>
                                                        <Text style={{ marginRight: 4 }}>{rank.emoji}</Text>
                                                        <Text style={{ color: rank.color, fontWeight: 'bold', fontSize: 12 }}>{rank.name} {score}</Text>
                                                    </View>
                                                )}
                                            </View>
                                            <Text style={{ color: THEME.colors.text.primary }} numberOfLines={1}>{item.team_name}</Text>
                                            <Text style={{ color: THEME.colors.text.secondary, fontSize: 12 }}>{item.location?.city}, {item.location?.region}</Text>
                                        </View>
                                        <ChevronRight color={THEME.colors.text.secondary} size={20} />
                                    </ScaleButton>
                                </AnimatedListItem>
                            );
                        }}
                        ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No teams found.</Text>}
                    />
                </View>

                <View style={{ flex: 1, display: activeTab === 'schedule' ? 'flex' : 'none' }}>
                    <FlatList
                        data={data.matches}
                        keyExtractor={item => item.id.toString()}
                        contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                        renderItem={({ item, index }) => {
                            const redAlliance = item.alliances.find(a => a.color === 'red');
                            const blueAlliance = item.alliances.find(a => a.color === 'blue');
                            const redScore = redAlliance?.score || 0;
                            const blueScore = blueAlliance?.score || 0;
                            const redWon = redScore > blueScore;
                            const blueWon = blueScore > redScore;

                            // Format time
                            const matchTime = item.scheduled ? new Date(item.scheduled).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', hour12: false }) : '';

                            const isVexIQ = event.program?.id === 41 || event.program?.code === 'VIQC';

                            if (isVexIQ) {
                                // VEX IQ Logic: Single score, collaborative
                                let matchName = item.name;
                                matchName = matchName
                                    .replace(/Practice\s?#?/, 'P')
                                    .replace(/Teamwork\s*(Match)?\s*#?/i, 'Q')
                                    .replace(/Match\s?#?/, 'F')
                                    .replace(/Qualifier\s?#?/, 'Q')
                                    .replace(/#/, '');

                                const teams = [...(redAlliance?.teams || []), ...(blueAlliance?.teams || [])];

                                return (
                                    <ScaleButton
                                        key={item.id}
                                        onPress={() => pushView('matchDetail', item)}
                                        style={[styles.listItem, { paddingVertical: 8 }]}>
                                        <View style={{ flexDirection: 'row', alignItems: 'center', flex: 1 }}>
                                            {/* Column 1: Name & Time */}
                                            <View style={{ width: 50, marginRight: 8 }}>
                                                <Text style={{ fontWeight: 'bold', color: THEME.colors.text.primary, fontSize: 14 }}>{matchName}</Text>
                                                {matchTime ? <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>{matchTime}</Text> : null}
                                            </View>

                                            {/* Column 2: Teams */}
                                            <View style={{ flex: 1, flexDirection: 'row', flexWrap: 'wrap', gap: 8 }}>
                                                {teams.map(t => (
                                                    <Text key={t.team.id} style={{ color: THEME.colors.blueAlliance, fontSize: 14, fontWeight: 'bold' }}>{t.team.name}</Text>
                                                ))}
                                            </View>

                                            {/* Column 3: Score */}
                                            <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.text.primary, width: 40, textAlign: 'right' }}>
                                                {(item.scored || item.started || (redAlliance?.score || 0) > 0) ? (redAlliance?.score || 0) : ''}
                                            </Text>

                                            <ChevronRight size={16} color={THEME.colors.text.secondary} style={{ marginLeft: 8 }} />
                                        </View>
                                    </ScaleButton>
                                );
                            }

                            let matchName = item.name;
                            matchName = matchName
                                .replace(/Practice\s?#?/, 'P')
                                .replace(/Teamwork\s*(Match)?\s*#?/i, 'Q')
                                .replace(/Qualification\s?#?/, 'Q')
                                .replace(/Qualifier\s?#?/, 'Q')
                                .replace(/Quarterfinal\s?#?/, 'QF')
                                .replace(/Semifinal\s?#?/, 'SF')
                                .replace(/Final\s?#?/, 'F')
                                .replace(/Round of 16\s?#?/, 'R16')
                                .replace(/#/, '');

                            return (
                                <ScaleButton style={[styles.listItem, { paddingVertical: 8 }]} onPress={() => pushView('matchDetail', item)}>
                                    <View style={{ flexDirection: 'row', alignItems: 'center', flex: 1 }}>
                                        {/* Column 1: Name & Time */}
                                        <View style={{ width: 50 }}>
                                            <Text style={{ fontWeight: 'bold', color: THEME.colors.text.primary, fontSize: 14 }}>{matchName}</Text>
                                            {matchTime ? <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>{matchTime}</Text> : null}
                                        </View>

                                        {/* Center Block */}
                                        <View style={{ flex: 1, flexDirection: 'row', justifyContent: 'center', alignItems: 'center' }}>
                                            {/* Red Teams */}
                                            <View style={{ width: 60, alignItems: 'flex-end', marginRight: 8 }}>
                                                {redAlliance?.teams.map(t => (
                                                    <Text key={t.team.id} style={{ color: THEME.colors.error, fontSize: 12 }}>{t.team.name}</Text>
                                                ))}
                                            </View>

                                            {/* Red Score */}
                                            <Text style={{
                                                color: THEME.colors.error,
                                                fontWeight: 'bold',
                                                fontSize: 16,
                                                width: 35,
                                                textAlign: 'center',
                                                textDecorationLine: redWon ? 'underline' : 'none',
                                            }}>
                                                {(item.scored || item.started || (redScore + blueScore !== 0)) ? redScore : ''}
                                            </Text>

                                            {/* Score Spacer */}
                                            <View style={{ width: 24 }} />

                                            {/* Blue Score */}
                                            <Text style={{
                                                color: THEME.colors.blueAlliance,
                                                fontWeight: 'bold',
                                                fontSize: 16,
                                                width: 35,
                                                textAlign: 'center',
                                                textDecorationLine: blueWon ? 'underline' : 'none',
                                            }}>
                                                {(item.scored || item.started || (redScore + blueScore !== 0)) ? blueScore : ''}
                                            </Text>

                                            {/* Blue Teams */}
                                            <View style={{ width: 60, alignItems: 'flex-start', marginLeft: 8 }}>
                                                {blueAlliance?.teams.map(t => (
                                                    <Text key={t.team.id} style={{ color: THEME.colors.blueAlliance, fontSize: 12 }}>{t.team.name}</Text>
                                                ))}
                                            </View>
                                        </View>

                                        <ChevronRight size={16} color={THEME.colors.text.secondary} />
                                    </View>
                                </ScaleButton>
                            );
                        }}
                        ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No matches found.</Text>}
                    />
                </View>

                <View style={{ flex: 1, display: activeTab === 'rankings' ? 'flex' : 'none' }}>
                    <View style={{ flex: 1 }}>
                        <View style={{ flexDirection: 'row', padding: 8, backgroundColor: THEME.colors.background }}>
                            {event.program?.id === 41 ? (
                                <ScaleButton
                                    onPress={() => setSortMethod('rank')}
                                    style={{
                                        flex: 1,
                                        padding: 8,
                                        backgroundColor: THEME.colors.primary,
                                        borderRadius: 8,
                                        alignItems: 'center',
                                        marginHorizontal: 4
                                    }}
                                >
                                    <Text style={{
                                        color: THEME.colors.background,
                                        fontWeight: 'bold',
                                        fontSize: 12
                                    }}>Rank</Text>
                                </ScaleButton>
                            ) : (
                                ['Rank', 'OPR', 'DPR', 'CCWM'].map(method => (
                                    <ScaleButton
                                        key={method}
                                        onPress={() => setSortMethod(method.toLowerCase())}
                                        style={{
                                            flex: 1,
                                            padding: 8,
                                            backgroundColor: sortMethod === method.toLowerCase() ? THEME.colors.primary : THEME.colors.surface,
                                            borderRadius: 8,
                                            alignItems: 'center',
                                            marginHorizontal: 4
                                        }}
                                    >
                                        <Text style={{
                                            color: sortMethod === method.toLowerCase() ? THEME.colors.background : THEME.colors.text.primary,
                                            fontWeight: 'bold',
                                            fontSize: 12
                                        }}>{method}</Text>
                                    </ScaleButton>
                                ))
                            )}
                        </View>
                        <FlatList
                            data={sortedRankings}
                            keyExtractor={item => item.id.toString()}
                            contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                            renderItem={({ item }) => {
                                const teamStats = stats[item.team.id] || { opr: 0, dpr: 0, ccwm: 0 };
                                return (
                                    <View style={styles.listItem}>
                                        <View style={{ flex: 1, flexDirection: 'row', alignItems: 'center', gap: 12, marginRight: 8 }}>
                                            <Text style={{ fontWeight: 'bold', width: 24, textAlign: 'center', color: THEME.colors.text.primary }}>{item.rank}</Text>
                                            <View style={{ flex: 1 }}>
                                                <Text style={{ fontWeight: 'bold', color: THEME.colors.primary }}>{item.team.name}</Text>
                                                <Text style={{ fontSize: 12, color: THEME.colors.text.secondary }} numberOfLines={1} ellipsizeMode="tail">
                                                    {teamNameMap[item.team.id] || ''}
                                                </Text>
                                                {event.program?.id !== 41 && (
                                                    <Text style={{ fontSize: 12, color: THEME.colors.text.secondary }}>
                                                        W-L-T: {item.wins}-{item.losses}-{item.ties}
                                                    </Text>
                                                )}
                                            </View>
                                        </View>
                                        <View style={{ alignItems: 'flex-end', minWidth: 100 }}>
                                            {event.program?.id === 41 ? (
                                                <Text style={{ fontWeight: 'bold', color: THEME.colors.text.primary }}>
                                                    Avg: {item.average_points || item.average_score || 0}
                                                </Text>
                                            ) : (
                                                <>
                                                    <Text style={{ fontWeight: 'bold', color: THEME.colors.text.primary }}>WP: {item.wp} | AP: {item.ap} | SP: {item.sp}</Text>
                                                    <Text style={{ fontSize: 10, color: THEME.colors.text.secondary }}>
                                                        OPR: {teamStats.opr.toFixed(1)} | DPR: {teamStats.dpr.toFixed(1)} | CCWM: {teamStats.ccwm.toFixed(1)}
                                                    </Text>
                                                </>
                                            )}
                                        </View>
                                    </View>
                                );
                            }}
                            ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No rankings found.</Text>}
                        />
                    </View>
                </View>

                <View style={{ flex: 1, display: activeTab === 'awards' ? 'flex' : 'none' }}>
                    <FlatList
                        data={data.awards}
                        keyExtractor={item => item.id.toString()}
                        contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                        renderItem={({ item }) => (
                            <View style={styles.listItem}>
                                <View style={{ flex: 1 }}>
                                    <Text style={{ fontWeight: 'bold', color: THEME.colors.primary }}>{item.title}</Text>
                                    {item.teamWinners && item.teamWinners.length > 0 && (
                                        <View style={{ marginTop: 4 }}>
                                            {item.teamWinners.map(winner => (
                                                <Text key={winner.team.id} style={{ color: THEME.colors.text.primary }}>
                                                    {winner.team.name} ({winner.division?.name || 'Event'})
                                                </Text>
                                            ))}
                                        </View>
                                    )}
                                </View>
                                <Award color={THEME.colors.primary} size={24} />
                            </View>
                        )}
                        ListEmptyComponent={<Text style={{ textAlign: 'center', marginTop: 20, color: THEME.colors.text.secondary }}>No awards found.</Text>}
                    />
                </View>
            </View>
        );
    };

    return (
        <View style={styles.viewContainer}>
            <View style={{ flexDirection: 'row', backgroundColor: THEME.colors.background, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                {tabs.map(tab => {
                    const Icon = tab.icon;
                    return (
                        <ScaleButton
                            key={tab.id}
                            onPress={() => setActiveTab(tab.id)}
                            style={{
                                flex: 1,
                                paddingVertical: 12,
                                alignItems: 'center',
                                borderBottomWidth: 2,
                                borderBottomColor: activeTab === tab.id ? THEME.colors.primary : 'transparent'
                            }}
                            activeScale={0.9}
                        >
                            <Icon size={20} color={activeTab === tab.id ? THEME.colors.primary : THEME.colors.text.secondary} style={{ marginBottom: 4 }} />
                            <Text style={{
                                color: activeTab === tab.id ? THEME.colors.primary : THEME.colors.text.secondary,
                                fontWeight: activeTab === tab.id ? '600' : '400',
                                fontSize: 10,
                                textTransform: 'uppercase'
                            }}>
                                {tab.label}
                            </Text>
                        </ScaleButton>
                    );
                })}
            </View>
            {renderContent()}
        </View >
    );
});

// --- Favorites View ---

const FavoritesView = React.memo(({ favorites, pushView }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);
    const sections = useMemo(() => {
        const { events } = favorites;
        const teams = [...favorites.teams].sort((a, b) => a.number.localeCompare(b.number, undefined, { numeric: true }));

        console.log("Favorites Teams:", JSON.stringify(teams, null, 2));

        const isVRC = (t) => t.program?.code === 'VRC' || t.program?.code === 'V5RC' || t.program?.id === 1;
        const isVIQC = (t) => t.program?.code === 'VIQC' || t.program?.id === 41;
        const isVEXU = (t) => t.program?.code === 'VEXU' || t.program?.id === 4;

        const isHS = (t) => t.grade === 'High School' || t.grade === 'HighSchool';
        const isMS = (t) => t.grade === 'Middle School' || t.grade === 'MiddleSchool';
        const isES = (t) => t.grade === 'Elementary School' || t.grade === 'ElementarySchool';

        const v5hs = teams.filter(t => isVRC(t) && isHS(t));
        const v5ms = teams.filter(t => isVRC(t) && isMS(t));
        const viqms = teams.filter(t => isVIQC(t) && isMS(t));
        const viqes = teams.filter(t => isVIQC(t) && isES(t));
        const vexu = teams.filter(t => isVEXU(t));

        // Categorize uncategorized teams (e.g. missing grade)
        const otherTeams = teams.filter(t =>
            !v5hs.includes(t) && !v5ms.includes(t) &&
            !viqms.includes(t) && !viqes.includes(t) &&
            !vexu.includes(t)
        );

        const result = [];
        if (v5hs.length > 0) result.push({ title: 'V5HS Team', data: v5hs, type: 'team' });
        if (v5ms.length > 0) result.push({ title: 'V5MS Team', data: v5ms, type: 'team' });
        if (viqms.length > 0) result.push({ title: 'VIQMS Team', data: viqms, type: 'team' });
        if (viqes.length > 0) result.push({ title: 'VIQES Team', data: viqes, type: 'team' });
        if (vexu.length > 0) result.push({ title: 'VEXU Team', data: vexu, type: 'team' });
        if (otherTeams.length > 0) result.push({ title: 'Other Teams', data: otherTeams, type: 'team' });
        if (events.length > 0) result.push({ title: 'Events', data: events, type: 'event' });

        return result;
    }, [favorites]);

    if (favorites.teams.length === 0 && favorites.events.length === 0) {
        return (
            <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center', padding: 32 }}>
                <Star size={64} color={THEME.colors.text.secondary} />
                <Text style={{ marginTop: 16, fontSize: 18, fontWeight: 'bold', color: THEME.colors.text.primary }}>No Favorites Yet</Text>
                <Text style={{ marginTop: 8, textAlign: 'center', color: THEME.colors.text.secondary }}>
                    Star teams and events to see them here.
                </Text>
            </View>
        );
    }

    return (
        <ScrollView style={styles.viewContainer} contentContainerStyle={{ padding: 16, paddingBottom: 100 }}>
            {sections.map((section, index) => (
                <View key={index} style={{ marginBottom: 24 }}>
                    <Text style={{ fontSize: 18, fontWeight: 'bold', marginBottom: 12, color: THEME.colors.text.primary }}>
                        {section.title}
                    </Text>
                    <View style={{ gap: 12 }}>
                        {section.data.map(item => (
                            <ScaleButton
                                key={item.id}
                                style={styles.card}
                                onPress={() => {
                                    if (section.type === 'team') {
                                        pushView('teamDetail', { team: item });
                                    } else {
                                        pushView('eventDetail', item);
                                    }
                                }}
                            >
                                {section.type === 'event' ? (
                                    <View style={{ flex: 1 }}>
                                        <Text style={{ fontWeight: 'bold', fontSize: 16, color: THEME.colors.text.primary }}>{item.name}</Text>
                                        <View style={{ flexDirection: 'row', alignItems: 'center', marginTop: 4 }}>
                                            <Calendar size={14} color={THEME.colors.text.secondary} />
                                            <Text style={{ color: THEME.colors.text.secondary, marginLeft: 4, fontSize: 12 }}>
                                                {new Date(item.start).toLocaleDateString()}
                                            </Text>
                                            <MapPin size={14} color={THEME.colors.text.secondary} style={{ marginLeft: 12 }} />
                                            <Text style={{ color: 'gray', marginLeft: 4, fontSize: 12 }}>
                                                {item.location?.city}, {item.location?.region}
                                            </Text>
                                        </View>
                                    </View>
                                ) : (

                                    <View style={styles.listItemContent}>
                                        <View style={{ flex: 1 }}>
                                            <Text style={styles.listItemTitle} numberOfLines={1}>
                                                <Text style={{ fontWeight: 'bold', color: THEME.colors.primary }}>{item.number}</Text>
                                                <Text>  {item.team_name}</Text>
                                            </Text>
                                            <Text style={styles.listItemSubtitle}>{item.organization} �?{item.location?.region}</Text>
                                        </View>
                                    </View>
                                )}
                                <ChevronRight color={THEME.colors.text.secondary} size={20} />
                            </ScaleButton>
                        ))}
                    </View>
                </View>
            ))}
        </ScrollView >
    );
});

// --- Main App Component ---

// --- The Infinite Scroller Component ---
const ScrollingText = ({ text, color, duration }) => {
    const translateX = useRef(new Animated.Value(0)).current;
    const [textWidth, setTextWidth] = useState(0);

    useEffect(() => {
        // Only start animation once we know the width of the text block
        if (textWidth > 0) {
            const animate = () => {
                translateX.setValue(0); // Start at 0
                Animated.loop(
                    Animated.timing(translateX, {
                        toValue: -textWidth, // Move left by exactly one text block length
                        duration: duration,
                        easing: Easing.linear,
                        useNativeDriver: true,
                    })
                ).start();
            };
            animate();
        }
    }, [textWidth, duration]);

    return (
        <View style={{ flexDirection: 'row', width: '200%' }}>
            {/* We wrap the text in an Animated.View.
               We render the SAME text block twice side-by-side.
               When the first block moves completely off-screen (translateX = -textWidth),
               the animation loops back to 0 immediately. 
            */}
            <Animated.View
                style={{
                    flexDirection: 'row',
                    transform: [{ translateX }]
                }}
            >
                {/* Copy 1: We measure this one */}
                <Text
                    onLayout={(e) => setTextWidth(e.nativeEvent.layout.width)}
                    style={[styles.text, { color: color }]}
                >
                    {text}
                </Text>

                {/* Copy 2: The follower (creates the illusion of infinity) */}
                <Text style={[styles.text, { color: color }]}>
                    {text}
                </Text>
            </Animated.View>
        </View>
    );
};

const LandscapeWatermark = React.memo(({ isLandscape, redScore, blueScore, mode }) => {
    // 1. Define base strings (single unit with spacing)
    // Note: Added spaces at the end for spacing between repeats
    let baseText = "TIE    ";

    let color = THEME.colors.text.primary; // Default/Theme color

    if (mode === 'match') {
        if (redScore > blueScore) {
            baseText = "RED WIN    ";
            color = THEME.colors.error;
        } else if (blueScore > redScore) {
            baseText = "BLUE WIN    ";
            color = THEME.colors.blueAlliance;
        }
    } else {
        baseText = "SKILLS    ";
    }

    // 2. Create the long string programmatically
    // Repeat enough times to ensure it covers the screen width easily
    const repeatedText = baseText.repeat(20);

    const { height } = useWindowDimensions();

    if (!isLandscape) return null;

    const lines = [
        { top: height * 0, duration: 25000 },
        { top: height * 0.70, duration: 20000 }, // Varied speed slightly for effect
    ];

    return (
        <View style={[StyleSheet.absoluteFill, { zIndex: 1, overflow: 'hidden' }]} pointerEvents="none">
            {lines.map((line, index) => (
                <View key={index} style={{ position: 'absolute', top: line.top, left: 0, right: 0 }}>
                    <ScrollingText
                        text={repeatedText}
                        color={color}
                        duration={line.duration}
                    />
                </View>
            ))}
        </View>
    );
});


const styles = StyleSheet.create({
    text: {
        fontSize: 80,
        fontWeight: '900',
        opacity: 0.5,
        letterSpacing: 15, // Keep letterSpacing consistent
        textTransform: 'uppercase',
    }
});

function MainContent() {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);

    // Core Score State (Moved up for CatTransition visibility)
    const [mode, setMode] = useState('match');
    const [goals, setGoals] = useState([
        { id: 1, redBlocks: 0, blueBlocks: 0, owner: null },
        { id: 2, redBlocks: 0, blueBlocks: 0, owner: null }
    ]);
    const [matchData, setMatchData] = useState({
        red: { autoBonus: false, blocks: 0, longGoals: 0, centerUpper: 0, centerLower: 0, parking: 0 },
        blue: { autoBonus: false, blocks: 0, longGoals: 0, centerUpper: 0, centerLower: 0, parking: 0 }
    });
    const [skillsGoals, setSkillsGoals] = useState([
        { id: 101, redBlocks: 0, blueBlocks: 0, owner: null },
        { id: 102, redBlocks: 0, blueBlocks: 0, owner: null }
    ]);
    const [skillsData, setSkillsData] = useState({
        centerUpper: 0,
        centerLower: 0,
        centerUpperOwned: null,
        centerLowerOwned: null,
        clearedLoaders: 0,
        clearedParking: 0,
        parking: 0
    });

    const calculateMatchScore = useCallback((alliance, data) => {
        let score = 0;
        const otherAlliance = alliance === 'red' ? 'blue' : 'red';
        const isTie = data.autoBonus && matchData[otherAlliance].autoBonus;
        if (isTie) score += 5;
        else if (data.autoBonus) score += 10;

        // Note: rings/blocks logic still uses 'goals' which is in ScoreCalculator
        // For the transition score, we'll use a snapshot or approximate if needed, 
        // but for now let's ensure the function is defined.
        const allianceBlocks = goals.reduce((sum, g) => sum + (alliance === 'red' ? g.redBlocks : g.blueBlocks), 0);
        score += allianceBlocks * 3;
        const ownedGoals = goals.filter(g => g.owner === alliance).length;
        score += ownedGoals * 10;
        score += (data.centerUpper || 0) * 3;
        score += (data.centerLower || 0) * 3;
        if ((data.centerLower || 0) > (matchData[otherAlliance].centerLower || 0)) score += 6;
        if ((data.centerUpper || 0) > (matchData[otherAlliance].centerUpper || 0)) score += 8;
        if (data.parking === 1) score += 8;
        if (data.parking === 2) score += 30;
        return score;
    }, [matchData, goals]);

    const calculateSkillsScore = useCallback((data) => {
        let score = 0;
        const blocksScore = skillsGoals.reduce((sum, g) => sum + (g.redBlocks || 0), 0);
        score += blocksScore * 1;
        const goalsPlacedScore = skillsGoals.filter(g => g.owner === 'red').length * 10;
        if (goalsPlacedScore > 0) score += (goalsPlacedScore / 2);
        if (data.centerUpperOwned === 'red') score += 10;
        if (data.centerLowerOwned === 'red') score += 10;
        score += (data.clearedLoaders || 0) * 5;
        score += (data.clearedParking || 0) * 5;
        score += (data.parking || 0) * 15;
        score += (data.centerUpper || 0) * 1;
        score += (data.centerLower || 0) * 1;
        return score;
    }, [skillsGoals]);

    console.log(" [DEBUG] App component is rendering!");

    const [fontsLoaded, fontError] = useFonts({
        Rajdhani_500Medium,
        Rajdhani_600SemiBold,
        Rajdhani_700Bold,
    });

    // Force hide splash screen on mount with a slight delay to ensure readiness
    useEffect(() => {
        const timer = setTimeout(() => {
            SplashScreen.hideAsync().catch(console.warn);
        }, 500);
        return () => clearTimeout(timer);
    }, []);

    // Also try to hide when fonts load, just in case
    useEffect(() => {
        if (fontsLoaded || fontError) {
            SplashScreen.hideAsync().catch(console.warn);
        }
    }, [fontsLoaded, fontError]);



    const [activeTab, setActiveTab] = useState('favorites');
    const [apiKey, setApiKey] = useState(ROBOT_EVENTS_API_KEY);
    const [seasons, setSeasons] = useState([]);
    const [selectedSeason, setSelectedSeason] = useState(null);
    const [favorites, setFavorites] = useState({ teams: [], events: [] });
    const [category, setCategory] = useState(CATEGORIES[0]);

    // Touch Debugger Logic
    const [debugDots, setDebugDots] = useState([]);
    const dotsRef = useRef([]);

    useEffect(() => {
        global.reportClick = (x, y, success) => {
            const now = Date.now();
            // Update the most recent dot if it's within a very short time window (200ms)
            // This is because the root onTouchStart and button handlePressIn happen in sequence
            setDebugDots(prev => {
                const latest = prev.length > 0 ? prev[prev.length - 1] : null;
                if (latest && (now - latest.id < 300)) {
                    // Update latest dot to success
                    const newDots = [...prev];
                    newDots[newDots.length - 1] = { ...latest, hit: true };
                    return newDots.slice(-50); // Keep last 50 dots
                }
                // Or if it's a direct hit reported without root firing (rare but possible)
                return [...prev, { x, y, hit: true, id: now }].slice(-50);
            });
        };
    }, []);

    const handleRootTouch = (e) => {
        const { pageX, pageY } = e.nativeEvent;
        const id = Date.now();
        setDebugDots(prev => [...prev, { x: pageX, y: pageY, hit: false, id }].slice(-50));
    };

    const handleCategorySelect = (newCategory) => {
        if (newCategory.id === category.id) return;

        // Only reset season if the program actually changes (e.g. VRC -> IQ)
        // If it's just a grade change (V5HS -> V5MS), we can keep the same season/seasons.
        if (newCategory.programId !== category.programId) {
            setSelectedSeason(null);
            setSeasons([]);
        }

        setCategory(newCategory);
    };

    // Load favorites on mount
    useEffect(() => {
        const loadFavorites = async () => {
            const favs = await FavoritesManager.getFavorites();
            setFavorites(favs);
        };
        loadFavorites();
    }, []);

    const toggleFavorite = async (item, type) => {
        const isFav = type === 'team'
            ? favorites.teams.some(t => t.id === item.id)
            : favorites.events.some(e => e.id === item.id);

        let newFavs;
        if (isFav) {
            newFavs = await FavoritesManager.removeFavorite(item.id, type);
        } else {
            newFavs = await FavoritesManager.addFavorite(item, type);
        }

        if (newFavs) {
            setFavorites(newFavs);
        }
    };

    // Removed AsyncStorage logic for API key since it's now hardcoded/configured

    const { request, loading } = useRobotEvents(apiKey);

    useEffect(() => {
        const fetchSeasons = async () => {
            if (!apiKey) return;
            // Fetch seasons for the selected program
            const data = await request('/seasons', { program: [category.programId] });
            if (data && data.data) {
                const sorted = data.data.sort((a, b) => b.id - a.id);
                setSeasons(sorted);
                // Default to the first one (newest)
                if (sorted.length > 0) {
                    setSelectedSeason(sorted[0]);
                } else {
                    setSelectedSeason(null);
                }
            }
        };
        fetchSeasons();
    }, [apiKey, request, category.programId]);

    const [navStacks, setNavStacks] = useState({
        events: [{ view: 'root' }],
        teams: [{ view: 'root' }],
        rankings: [{ view: 'root' }],
        favorites: [{ view: 'root' }],
        settings: [{ view: 'root' }],
        predict: [{ view: 'root' }]
    });

    // Track the last active main tab to know which stack to push/pop from if currently on overlay
    // However, overlay tabs (calculator/manual) don't have stacks, so we only care about main tabs.
    // We can use stackTab for this.

    const [stackTab, setStackTab] = useState('favorites');



    const handleTabChange = (tabId) => {
        const isOverlay = (t) => t === 'calculator' || t === 'manual';

        if (isOverlay(tabId)) {
            setActiveTab(tabId);
            return;
        }

        // Switching to a main tab
        if (tabId !== stackTab) {
            // Switching between different main tabs -> Just switch, don't reset
            setStackTab(tabId);
        } else if (!isOverlay(activeTab)) {
            // Clicking the same main tab again (and not coming from overlay) -> Pop to root
            setNavStacks(prev => ({
                ...prev,
                [tabId]: [{ view: 'root' }]
            }));
        }

        setActiveTab(tabId);
    };

    const pushToStack = useCallback((tabId, view, data) => {
        setNavStacks(prev => ({
            ...prev,
            [tabId]: [...prev[tabId], { view, data }]
        }));
    }, []);

    const pushToEvents = useCallback((view, data) => pushToStack('events', view, data), [pushToStack]);
    const pushToTeams = useCallback((view, data) => pushToStack('teams', view, data), [pushToStack]);
    const pushToRankings = useCallback((view, data) => pushToStack('rankings', view, data), [pushToStack]);
    const pushToFavorites = useCallback((view, data) => pushToStack('favorites', view, data), [pushToStack]);

    const pushView = (view, data) => {
        // If we are on an overlay tab (calculator/manual), switch to the stack tab so we can see the pushed view
        if (activeTab === 'calculator' || activeTab === 'manual') {
            setActiveTab(stackTab);
        }
        // Always push to the currently active stack (stackTab)
        pushToStack(stackTab, view, data);
    };

    const popView = () => {
        setNavStacks(prev => {
            const currentStack = prev[stackTab];
            if (currentStack.length > 1) {
                // Mark the top view as closing
                const newStack = [...currentStack];
                newStack[newStack.length - 1] = { ...newStack[newStack.length - 1], closing: true };
                return {
                    ...prev,
                    [stackTab]: newStack
                };
            }
            return prev;
        });
    };

    const removeView = (tabId) => {
        setNavStacks(prev => {
            const currentStack = prev[tabId];
            if (currentStack.length > 1) {
                // Actually remove the view
                return {
                    ...prev,
                    [tabId]: currentStack.slice(0, -1)
                };
            }
            return prev;
        });
    };

    const renderTabStack = (tabId) => {
        const stack = navStacks[tabId];
        const pushFn = tabId === 'events' ? pushToEvents :
            tabId === 'teams' ? pushToTeams :
                tabId === 'rankings' ? pushToRankings :
                    tabId === 'favorites' ? pushToFavorites :
                        pushView;

        // Render the stack, ensuring proper z-index and positioning
        return stack.map((currentView, index) => {
            // Only render the top 2 views to save memory/performance (current and previous)
            // Or if a view is closing, we need to see the one below it.
            if (index < stack.length - 2) return null;

            const isRoot = index === 0;
            const isTop = index === stack.length - 1;

            // Root view is always rendered normally.
            // Pushed views are absolute positioned to cover the screen.
            const style = isRoot ? { flex: 1 } : {
                position: 'absolute',
                top: 0,
                left: 0,
                right: 0,
                bottom: 0,
                zIndex: 100 + index // Ensure pushed views are above root view elements (like zIndexed filters)
            };

            // Detail Views
            // Detail Views
            if (currentView.view === 'eventDetail') {
                const event = currentView.data;
                return (
                    <ScreenTransition
                        key={index}
                        style={style}
                        visible={!currentView.closing}
                        onExited={() => removeView(tabId)}
                        onSwipeBack={popView}
                    >
                        <EventDetailView
                            event={event}
                            apiKey={apiKey}
                            apiRequest={request}
                            pushView={pushFn}
                            isFavorite={favorites.events.some(e => e.id === event.id)}
                            onToggleFavorite={toggleFavorite}
                        />
                    </ScreenTransition>
                );
            }
            if (currentView.view === 'teamDetail') {
                // currentView.data can be just a team object, or {team, event, division}
                const teamData = currentView.data.team ? currentView.data : { team: currentView.data };

                return (
                    <ScreenTransition
                        key={index}
                        style={style}
                        visible={!currentView.closing}
                        onExited={() => removeView(tabId)}
                        onSwipeBack={popView}
                    >
                        <TeamInfoView
                            {...teamData}
                            apiKey={apiKey}
                            apiRequest={request}
                            pushView={pushFn}
                            selectedSeason={selectedSeason}
                            isFavorite={favorites.teams.some(t => t.id === teamData.team.id)}
                            onToggleFavorite={toggleFavorite}
                        />
                    </ScreenTransition>
                );
            }
            if (currentView.view === 'teamAwards') {
                return (
                    <ScreenTransition
                        key={index}
                        style={style}
                        visible={!currentView.closing}
                        onExited={() => removeView(tabId)}
                        onSwipeBack={popView}
                    >
                        <TeamAwardsView team={currentView.data.team} awards={currentView.data.awards} season={currentView.data.season} />
                    </ScreenTransition>
                );
            }
            if (currentView.view === 'divisionDetail') {
                return (
                    <ScreenTransition
                        key={index}
                        style={style}
                        visible={!currentView.closing}
                        onExited={() => removeView(tabId)}
                        onSwipeBack={popView}
                    >
                        <DivisionDetailView event={currentView.data.event} division={currentView.data.division} apiKey={apiKey} apiRequest={request} pushView={pushFn} />
                    </ScreenTransition>
                );
            }

            if (currentView.view === 'matchDetail') {
                return (
                    <ScreenTransition
                        key={index}
                        style={style}
                        visible={!currentView.closing}
                        onExited={() => removeView(tabId)}
                        onSwipeBack={popView}
                    >
                        <MatchDetailView match={currentView.data} pushView={pushFn} />
                    </ScreenTransition>
                );
            }

            if (currentView.view === 'settings') {
                return (
                    <ScreenTransition
                        key={index}
                        style={style}
                        visible={!currentView.closing}
                        onExited={() => removeView(tabId)}
                        onSwipeBack={popView}
                    >
                        <SettingsView apiKey={apiKey} setApiKey={setApiKey} seasons={seasons} selectedSeason={selectedSeason} onSelectSeason={setSelectedSeason} />
                    </ScreenTransition>
                );
            }

            // Root Views
            if (tabId === 'events') {
                return (
                    <EventList
                        key={index}
                        onSelectEvent={e => pushToEvents('eventDetail', e)}
                        apiKey={apiKey}
                        apiRequest={request}
                        loading={loading}
                        selectedSeason={selectedSeason}
                        category={category}
                        setCategory={handleCategorySelect}
                    />
                );
            }
            if (tabId === 'teams') {
                return (
                    <TeamList
                        key={index}
                        onSelectTeam={t => pushToTeams('teamDetail', t)}
                        apiKey={apiKey}
                        apiRequest={request}
                        loading={loading}
                        selectedSeason={selectedSeason}
                        category={category}
                        setCategory={handleCategorySelect}
                    />
                );
            }
            if (tabId === 'rankings') {
                return (
                    <Rankings
                        key={index}
                        apiKey={apiKey}
                        apiRequest={request}
                        selectedSeason={selectedSeason}
                        category={category}
                    />
                );
            }
            if (tabId === 'favorites') {
                return <FavoritesView key={index} favorites={favorites} pushView={pushToFavorites} />;
            }
            return null;
        });
    };

    const renderContent = () => {
        const isOverlay = activeTab === 'calculator' || activeTab === 'manual';
        return (
            <View style={{ flex: 1 }}>
                <TabFadeView active={activeTab === 'calculator'}>
                    <ScoreCalculator
                        mode={mode} setMode={setMode}
                        goals={goals} setGoals={setGoals}
                        matchData={matchData} setMatchData={setMatchData}
                        skillsData={skillsData} setSkillsData={setSkillsData}
                        skillsGoals={skillsGoals} setSkillsGoals={setSkillsGoals}
                        calculateMatchScore={calculateMatchScore}
                        calculateSkillsScore={calculateSkillsScore}
                    />
                </TabFadeView>

                <TabFadeView active={activeTab === 'manual'}>
                    <ManualView />
                </TabFadeView>

                <TabFadeView active={!isOverlay}>
                    <TabFadeView active={stackTab === 'events'}>
                        {renderTabStack('events')}
                    </TabFadeView>
                    <TabFadeView active={stackTab === 'teams'}>
                        {renderTabStack('teams')}
                    </TabFadeView>
                    <TabFadeView active={stackTab === 'rankings'}>
                        {renderTabStack('rankings')}
                    </TabFadeView>
                    <TabFadeView active={stackTab === 'favorites'}>
                        {renderTabStack('favorites')}
                    </TabFadeView>
                    <TabFadeView active={stackTab === 'settings'}>
                        {renderTabStack('settings')}
                    </TabFadeView>
                </TabFadeView>
            </View>
        );
    };

    const getTitle = () => {
        if (activeTab === 'manual') return 'Game Manual';
        if (activeTab === 'calculator') return 'Score Calculator';

        const stack = navStacks[stackTab];
        const currentView = stack[stack.length - 1];

        if (currentView.view === 'eventDetail') return 'Event Details';
        if (currentView.view === 'divisionDetail') return currentView.data.division.name;
        if (currentView.view === 'matchDetail') return 'Match Details';
        if (currentView.view === 'teamDetail') return currentView.data.number;
        if (currentView.view === 'settings') return 'Settings';
        return stackTab.charAt(0).toUpperCase() + stackTab.slice(1);
    };

    const tabs = [
        { id: 'events', label: 'Events', icon: Calendar },
        { id: 'teams', label: 'Teams', icon: Users },
        { id: 'rankings', label: 'Ranks', icon: Trophy },
        { id: 'favorites', label: 'Saved', icon: Star },
        { id: 'manual', label: 'Rules', icon: BookOpen },
        { id: 'calculator', label: 'Score', icon: Calculator },
    ];



    // Removed font loading guard to ensure app always renders

    const { width, height } = useWindowDimensions();
    const isLandscape = width > height;

    return (
        <SafeAreaProvider onTouchStart={handleRootTouch}>
            <View style={styles.container}>
                <LandscapeWatermark
                    isLandscape={isLandscape}
                    redScore={mode === 'match' ? calculateMatchScore('red', matchData.red) : calculateSkillsScore(skillsData)}
                    blueScore={mode === 'match' ? calculateMatchScore('blue', matchData.blue) : 0}
                    mode={mode}
                />

                {activeTab !== 'manual' && !(activeTab === 'calculator' && isLandscape) && (
                    <Header
                        title={getTitle()}
                        showBack={navStacks[stackTab] && navStacks[stackTab].length > 1}
                        onBack={popView}
                        rightElement={
                            navStacks[stackTab][navStacks[stackTab].length - 1].view !== 'settings' ? (
                                <ScaleButton
                                    onPress={() => pushView('settings')}
                                    style={{ padding: 12 }}
                                    hitSlop={{ top: 20, bottom: 20, left: 20, right: 20 }}
                                >
                                    <View pointerEvents="none">
                                        <Settings size={28} color={THEME.colors.primary} />
                                    </View>
                                </ScaleButton>
                            ) : null
                        }
                    />
                )}
                <View style={styles.mainContent}>
                    {renderContent()}
                </View>

                {/* Touch Debug Overlay */}
                <View pointerEvents="box-none" style={[StyleSheet.absoluteFill, { zIndex: 99999 }]}>
                    {debugDots.map(dot => (
                        <View
                            key={dot.id}
                            pointerEvents="none"
                            style={{
                                position: 'absolute',
                                left: dot.x - 6,
                                top: dot.y - 6,
                                width: 12,
                                height: 12,
                                borderRadius: 6,
                                backgroundColor: dot.hit ? '#10b981' : '#ef4444',
                                borderWidth: 2,
                                borderColor: 'white',
                                opacity: 0.8,
                                shadowColor: '#000',
                                shadowOffset: { width: 0, height: 2 }, shadowOpacity: 0.3, shadowRadius: 3,
                                elevation: 5
                            }}
                        />
                    ))}
                    {debugDots.length > 0 && (
                        <Pressable
                            onPress={() => setDebugDots([])}
                            style={{ position: 'absolute', top: 140, right: 20, backgroundColor: 'rgba(0,0,0,0.6)', paddingHorizontal: 12, paddingVertical: 6, borderRadius: 20 }}
                        >
                            <Text style={{ color: 'white', fontSize: 10, fontWeight: 'bold' }}>Clear Debug Dots</Text>
                        </Pressable>
                    )}
                </View>

                {!(activeTab === 'calculator' && isLandscape) && <TabBar activeTab={activeTab} onTabChange={handleTabChange} tabs={tabs} />}
            </View>
        </SafeAreaProvider>
    );
}

const getStyles = (THEME) => StyleSheet.create({
    container: { flex: 1, backgroundColor: THEME.colors.background },
    mainContent: { flex: 1 },
    viewContainer: { flex: 1, backgroundColor: THEME.colors.background },

    // Cards
    card: {
        backgroundColor: THEME.colors.surface,
        padding: 16,
        borderRadius: THEME.borderRadius.m,
        marginBottom: 8,
        flexDirection: 'row',
        alignItems: 'center',
        justifyContent: 'space-between'
    },
    cardTitle: { fontSize: 16, fontWeight: '600', color: THEME.colors.text.primary, marginBottom: 4 },
    cardSubtitle: { fontSize: 12, fontWeight: 'bold', color: THEME.colors.primary, marginBottom: 4 },
    cardFooter: { flexDirection: 'row', gap: 12 },
    cardFooterItem: { flexDirection: 'row', alignItems: 'center', gap: 4 },
    cardFooterText: { fontSize: 12, color: THEME.colors.text.secondary },

    // List Items
    listItem: {
        backgroundColor: THEME.colors.surface,
        padding: 16,
        borderBottomWidth: 1,
        borderBottomColor: THEME.colors.border,
        flexDirection: 'row',
        alignItems: 'center',
        justifyContent: 'space-between'
    },
    listItemContent: { flexDirection: 'row', alignItems: 'center', gap: 16, flex: 1 },
    listItemTitle: { fontSize: 16, fontWeight: '600', color: THEME.colors.text.primary },
    listItemSubtitle: { fontSize: 12, color: THEME.colors.text.secondary },
    teamNumberText: { fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, width: 60 },
    teamBadgeLarge: {
        width: 80,
        height: 80,
        borderRadius: 16,
        backgroundColor: THEME.colors.primary,
        alignItems: 'center',
        justifyContent: 'center',
        marginBottom: 16
    },
    teamBadgeTextLarge: { color: THEME.colors.text.inverse, fontWeight: 'bold', fontSize: 24 },

    // Search
    searchHeader: { padding: 16, backgroundColor: THEME.colors.surface, borderBottomWidth: 1, borderBottomColor: THEME.colors.border },
    searchBar: { flexDirection: 'row', alignItems: 'center', backgroundColor: THEME.colors.background, borderRadius: 12, paddingHorizontal: 12, height: 40 },
    searchInput: { flex: 1, marginLeft: 8, fontSize: 14, color: THEME.colors.text.primary },

    // Settings
    infoBox: {
        flexDirection: 'row',
        gap: 12,
        backgroundColor: THEME.colors.surface,
        padding: 16,
        borderRadius: 12,
        borderWidth: 1,
        borderColor: THEME.colors.border,
        marginBottom: 24
    },
    infoBoxTitle: { fontSize: 14, fontWeight: '600', color: THEME.colors.primary },
    infoBoxText: { fontSize: 12, color: THEME.colors.text.secondary, marginTop: 4 },
    sectionTitle: { fontSize: 14, fontWeight: '600', color: THEME.colors.text.secondary, textTransform: 'uppercase', marginBottom: 8, marginLeft: 4 },
    formCard: { backgroundColor: THEME.colors.surface, borderRadius: 12, borderWidth: 1, borderColor: THEME.colors.border, overflow: 'hidden' },
    formItem: { padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border },
    label: { fontSize: 12, fontWeight: '500', color: THEME.colors.text.secondary, marginBottom: 4 },
    input: { fontSize: 14, color: THEME.colors.text.primary },
    formFooter: { padding: 16, backgroundColor: THEME.colors.surface },
    formFooterText: { fontSize: 10, color: THEME.colors.text.secondary },
    versionText: { textAlign: 'center', marginTop: 32, fontSize: 12, color: THEME.colors.text.secondary },

    // Calculator
    modeSwitcher: { padding: 16, backgroundColor: THEME.colors.surface, borderBottomWidth: 1, borderBottomColor: THEME.colors.border },
    modeSwitcherInner: { flexDirection: 'row', backgroundColor: THEME.colors.background, padding: 4, borderRadius: 8 },
    modeButton: { flex: 1, paddingVertical: 6, alignItems: 'center', borderRadius: 6 },
    modeButtonActive: { backgroundColor: THEME.colors.surface, shadowColor: '#000', shadowOffset: { width: 0, height: 1 }, shadowOpacity: 0.1, shadowRadius: 1 },
    modeButtonText: { fontSize: 14, fontWeight: '600', color: THEME.colors.text.secondary },
    modeButtonTextActive: { color: THEME.colors.text.primary },
    scoreHeader: { flexDirection: 'row', alignItems: 'center', padding: 16, backgroundColor: THEME.colors.surface, borderBottomWidth: 1, borderBottomColor: THEME.colors.border, gap: 16 },
    matchScoreContainer: { flex: 1, flexDirection: 'row', gap: 16 },
    scoreCard: { flex: 1, padding: 12, alignItems: 'center' },
    scoreLabel: { fontSize: 12, fontWeight: 'bold', marginBottom: 4, color: THEME.colors.text.secondary },
    scoreValue: { fontSize: 30, fontWeight: 'bold', color: THEME.colors.text.primary },
    awpBadge: { backgroundColor: THEME.colors.primary, paddingHorizontal: 6, paddingVertical: 2, borderRadius: 4, marginTop: 4 },
    awpText: { fontSize: 10, fontWeight: 'bold', color: THEME.colors.text.inverse },
    skillsScoreCard: { flex: 1, backgroundColor: THEME.colors.surface, padding: 16, borderRadius: 12, flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' },
    skillsScoreLabel: { color: THEME.colors.text.primary, fontWeight: '500' },
    skillsScoreValue: { color: THEME.colors.text.primary, fontSize: 30, fontWeight: 'bold' },
    resetButton: { padding: 8, backgroundColor: THEME.colors.surface, borderRadius: 20 },
    calculatorBody: { padding: 16, gap: 16 },
    sectionCard: { backgroundColor: THEME.colors.surface, borderRadius: 12, borderWidth: 1, borderColor: THEME.colors.border, overflow: 'hidden' },
    sectionHeader: { backgroundColor: THEME.colors.background, paddingHorizontal: 16, paddingVertical: 8, borderBottomWidth: 1, borderBottomColor: THEME.colors.border },
    sectionHeaderText: { fontSize: 12, fontWeight: 'bold', color: THEME.colors.text.secondary, textTransform: 'uppercase' },
    row: { flexDirection: 'row' },
    col: { flex: 1, padding: 12, gap: 12, borderRightWidth: 1, borderRightColor: THEME.colors.border },
    inputRow: { flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' },
    inputLabel: { fontSize: 14, color: THEME.colors.text.primary },
    checkbox: { width: 20, height: 20, borderRadius: 4, borderWidth: 1, borderColor: THEME.colors.text.secondary },
    stepperRow: { gap: 4 },
    parkingToggle: { flexDirection: 'row', backgroundColor: THEME.colors.background, borderRadius: 8, padding: 4 },
    parkingButton: { flex: 1, paddingVertical: 4, alignItems: 'center', borderRadius: 6 },
    parkingButtonActive: { backgroundColor: THEME.colors.surface },
    parkingButtonText: { fontSize: 12, fontWeight: 'bold', color: THEME.colors.text.secondary },
    parkingButtonTextActive: { color: THEME.colors.text.primary },
    parkingScoreText: { textAlign: 'center', fontSize: 10, color: THEME.colors.text.secondary, marginTop: 8 },
    skillsRow: { flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', padding: 16, borderBottomWidth: 1, borderBottomColor: THEME.colors.border },
    skillsLabel: { fontSize: 14, fontWeight: '500', color: THEME.colors.text.primary },
    toggleSwitch: { width: 44, height: 24, backgroundColor: THEME.colors.border, borderRadius: 12, padding: 2 },
    toggleSwitchActive: { backgroundColor: THEME.colors.primary },
    toggleKnob: { width: 20, height: 20, backgroundColor: THEME.colors.text.inverse, borderRadius: 10 },
    toggleKnobActive: { transform: [{ translateX: 20 }] },

    // Manual
    manualSection: { marginBottom: 32 },
    manualSectionHeader: { flexDirection: 'row', alignItems: 'center', marginBottom: 16 },
    manualSectionIndicator: { width: 4, height: 32, backgroundColor: THEME.colors.primary, borderRadius: 2, marginRight: 8 },
    manualSectionTitle: { fontSize: 20, fontWeight: 'bold', color: THEME.colors.text.primary, textTransform: 'uppercase' },
    ruleCard: {
        backgroundColor: THEME.colors.surface,
        padding: 20,
        borderRadius: 12,
        marginBottom: 16,
        shadowColor: '#000',
        shadowOffset: { width: 0, height: 1 },
        shadowOpacity: 0.05,
        shadowRadius: 2
    },
    ruleHeader: { flexDirection: 'row', alignItems: 'flex-start', gap: 12, marginBottom: 8 },
    ruleCodeBadge: { backgroundColor: THEME.colors.background, paddingHorizontal: 8, paddingVertical: 4, borderRadius: 4, minWidth: 56, alignItems: 'center' },
    ruleCodeText: { color: THEME.colors.primary, fontWeight: 'bold', fontSize: 14 },
    ruleName: { fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, flex: 1 },
    ruleText: { fontSize: 14, color: THEME.colors.text.secondary, lineHeight: 22 },
});

const GoalWidget = React.memo(({ goal, updateGoal, isSkills = false }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);

    // Long Press Timer Logic
    const timerRef = React.useRef(null);
    const redBlocksRef = React.useRef(goal.redBlocks);
    const blueBlocksRef = React.useRef(goal.blueBlocks);
    // Keep rings refs if needed, or remove if unused. Assuming used elsewhere or legacy?
    // Actually the startTimer uses redBlocksRef/blueBlocksRef.

    React.useEffect(() => { redBlocksRef.current = goal.redBlocks; }, [goal.redBlocks]);
    React.useEffect(() => { blueBlocksRef.current = goal.blueBlocks; }, [goal.blueBlocks]);

    const stopTimer = () => {
        if (timerRef.current) {
            clearInterval(timerRef.current);
            timerRef.current = null;
        }
    };

    const updateGoalRef = React.useRef(updateGoal);
    React.useEffect(() => { updateGoalRef.current = updateGoal; }, [updateGoal]);

    // Cleanup timer on unmount
    React.useEffect(() => () => stopTimer(), []);

    const startTimer = (field, delta) => {
        stopTimer();
        timerRef.current = setInterval(() => {
            const currentVal = field === 'redBlocks' ? redBlocksRef.current : blueBlocksRef.current;
            const nextVal = currentVal + delta;

            // Check total capacity
            const otherVal = field === 'redBlocks' ? (isSkills ? 0 : blueBlocksRef.current) : redBlocksRef.current;
            const newTotal = nextVal + otherVal;

            if (nextVal >= 0 && newTotal <= 15) {
                updateGoalRef.current(goal.id, field, nextVal);
            }
        }, 100);
    };

    const isRedOwned = goal.owner === 'red';
    const isBlueOwned = goal.owner === 'blue';
    const tintColor = isRedOwned ? THEME.colors.error : (isBlueOwned ? THEME.colors.blueAlliance : null);

    // Check if goal is at capacity (15 blocks)
    const totalBlocks = isSkills ? (goal.redBlocks || 0) : ((goal.redBlocks || 0) + (goal.blueBlocks || 0));
    const isFull = totalBlocks >= 15;
    const dimmerStyle = isFull ? { opacity: 0.3 } : {};

    return (
        <View
            pointerEvents="box-none"
            style={{
                padding: 2, marginBottom: -25, zIndex: 20
            }}
        >

            {/* Top Row: Red Blocks - Image - Blue Blocks */}
            <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'center', gap: 16, marginBottom: 0 }} pointerEvents="box-none">
                {/* Red Blocks (Vertical) */}
                <View style={{ height: 135, justifyContent: 'center', alignItems: 'center' }} pointerEvents="box-none">
                    <View style={{ alignItems: 'center', gap: 0 }}>
                        <ScaleButton
                            disabled={isFull}
                            onPress={() => updateGoal(goal.id, 'redBlocks', goal.redBlocks + 1)}
                            onLongPress={() => startTimer('redBlocks', 1)}
                            onPressOut={() => stopTimer()}
                            style={[styles.stepperButtonActive, { width: 28, height: 28, borderRadius: 8, backgroundColor: THEME.colors.error, borderColor: THEME.colors.error, justifyContent: 'center', alignItems: 'center' }, dimmerStyle]}
                        >
                            <View pointerEvents="none"><Plus size={16} color="white" /></View>
                        </ScaleButton>
                        <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>
                            {goal.redBlocks}
                        </Text>
                        <ScaleButton
                            onPress={() => updateGoal(goal.id, 'redBlocks', goal.redBlocks - 1)}
                            onLongPress={() => startTimer('redBlocks', -1)}
                            onPressOut={() => stopTimer()}
                            hitSlop={{ top: 25, bottom: 25, left: 25, right: 25 }}
                            style={[styles.stepperButton, { width: 28, height: 28, borderRadius: 8, backgroundColor: THEME.colors.error + '20', borderWidth: 2, borderColor: THEME.colors.error, justifyContent: 'center', alignItems: 'center' }]}>
                            <View pointerEvents="none"><Minus size={16} color={THEME.colors.text.primary} /></View>
                        </ScaleButton>
                    </View>
                </View>

                {/* Image Center & Overlay Buttons */}
                <View style={{ width: 180, height: 135, alignItems: 'center', justifyContent: 'center', marginHorizontal: isSkills ? 16 : 0 }}>
                    {/* Base Goal Image */}
                    <Image
                        source={ASSETS.longGoal}
                        fadeDuration={0}
                        pointerEvents="none"
                        style={{
                            position: 'absolute', top: 17.5, left: -5, width: 190, height: 95, resizeMode: 'contain',
                        }}
                    />

                    {/* Overlays for Ownership */}
                    <Image
                        source={ASSETS.redLongGoal}
                        fadeDuration={0}
                        pointerEvents="none"
                        style={{
                            position: 'absolute', top: 17.5, left: -5, width: 190, height: 95, resizeMode: 'contain',
                            opacity: isRedOwned ? 1 : 0
                        }}
                    />
                    {!isSkills && (
                        <Image
                            source={ASSETS.blueLongGoal}
                            fadeDuration={0}
                            pointerEvents="none"
                            style={{
                                position: 'absolute', top: 17.5, left: -5, width: 190, height: 95, resizeMode: 'contain',
                                opacity: isBlueOwned ? 1 : 0
                            }}
                        />
                    )}

                    {/* Red Overlay Button (Bottom-Aligned with PNG) */}
                    <ScaleButton
                        onPress={() => updateGoal(goal.id, 'owner', isRedOwned ? null : 'red')}
                        hitSlop={{ top: 10, bottom: 10, left: 10, right: 5 }}
                        style={{ position: 'absolute', bottom: 50, left: 45, zIndex: 50 }}
                    >
                        <View style={{
                            width: 28, height: 28, borderRadius: 8,
                            backgroundColor: isRedOwned ? THEME.colors.error : 'rgba(239, 68, 68, 0.15)',
                            borderWidth: 2, borderColor: THEME.colors.error,
                            justifyContent: 'center', alignItems: 'center',
                            elevation: 4, shadowColor: '#000', shadowOffset: { width: 0, height: 2 }, shadowOpacity: 0.2, shadowRadius: 4
                        }}>
                            <ArrowLeft size={16} color={isRedOwned ? 'white' : THEME.colors.error} />
                        </View>
                    </ScaleButton>

                    {/* Trash Button (Bottom-Aligned with PNG) */}
                    <ScaleButton
                        onPress={() => updateGoal(goal.id, 'reset')}
                        hitSlop={{ top: 10, bottom: 10, left: 5, right: 5 }}
                        style={{ position: 'absolute', bottom: 52, left: '50%', transform: [{ translateX: -14 }], zIndex: 50 }}
                    >
                        <View style={{
                            width: 28, height: 28, borderRadius: 14,
                            backgroundColor: THEME.colors.surface,
                            borderWidth: 1, borderColor: THEME.colors.border,
                            justifyContent: 'center', alignItems: 'center',
                            shadowColor: '#000', shadowOpacity: 0.1, shadowRadius: 4, elevation: 3
                        }}>
                            <View pointerEvents="none">
                                <Trash2 size={16} color={THEME.colors.text.secondary} />
                            </View>
                        </View>
                    </ScaleButton>

                    {/* Blue Overlay Button (Bottom-Aligned with PNG) */}
                    {!isSkills && (
                        <ScaleButton
                            onPress={() => updateGoal(goal.id, 'owner', isBlueOwned ? null : 'blue')}
                            hitSlop={{ top: 10, bottom: 10, left: 5, right: 10 }}
                            style={{ position: 'absolute', bottom: 50, right: 45, zIndex: 50 }}
                        >
                            <View style={{
                                width: 28, height: 28, borderRadius: 8,
                                backgroundColor: isBlueOwned ? THEME.colors.blueAlliance : 'rgba(59, 130, 246, 0.15)',
                                borderWidth: 2, borderColor: THEME.colors.blueAlliance,
                                justifyContent: 'center', alignItems: 'center',
                                elevation: 4, shadowColor: '#000', shadowOffset: { width: 0, height: 2 }, shadowOpacity: 0.2, shadowRadius: 4
                            }}>
                                <ArrowRight size={16} color={isBlueOwned ? 'white' : THEME.colors.blueAlliance} />
                            </View>
                        </ScaleButton>
                    )}
                </View>

                {/* Blue Blocks - Hidden in Skills */}
                {!isSkills && (
                    <View style={{ height: 135, justifyContent: 'center', alignItems: 'center' }} pointerEvents="box-none">
                        <View style={{ alignItems: 'center', gap: 0 }}>
                            <ScaleButton
                                disabled={isFull}
                                onPress={() => updateGoal(goal.id, 'blueBlocks', goal.blueBlocks + 1)}
                                onLongPress={() => startTimer('blueBlocks', 1)}
                                onPressOut={() => stopTimer()}
                                style={[styles.stepperButtonActive, { width: 28, height: 28, borderRadius: 8, backgroundColor: THEME.colors.blueAlliance, borderColor: THEME.colors.blueAlliance, justifyContent: 'center', alignItems: 'center' }, dimmerStyle]}
                            >
                                <View pointerEvents="none"><Plus size={16} color="white" /></View>
                            </ScaleButton>
                            <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>
                                {goal.blueBlocks}
                            </Text>
                            <ScaleButton
                                onPress={() => updateGoal(goal.id, 'blueBlocks', goal.blueBlocks - 1)}
                                onLongPress={() => startTimer('blueBlocks', -1)}
                                onPressOut={() => stopTimer()}
                                hitSlop={{ top: 25, bottom: 25, left: 25, right: 25 }}
                                style={[styles.stepperButton, { width: 28, height: 28, borderRadius: 8, backgroundColor: THEME.colors.blueAlliance + '20', borderWidth: 2, borderColor: THEME.colors.blueAlliance, justifyContent: 'center', alignItems: 'center' }]}>
                                <View pointerEvents="none"><Minus size={16} color={THEME.colors.text.primary} /></View>
                            </ScaleButton>
                        </View>
                    </View>
                )}
            </View>
        </View>
    );
});

const CenterGoalWidget = React.memo(({ matchData, updateMatch, resetCenterGoals, isSkills = false }) => {
    const { theme: THEME } = useTheme();

    // Long Press Timer Logic (moved to component level like GoalWidget)
    const timerRef = React.useRef(null);
    const updateMatchRef = React.useRef(updateMatch);
    React.useEffect(() => { updateMatchRef.current = updateMatch; }, [updateMatch]);

    const stopTimer = () => {
        if (timerRef.current) {
            clearInterval(timerRef.current);
            timerRef.current = null;
        }
    };

    // Cleanup timer on unmount
    React.useEffect(() => () => stopTimer(), []);

    const startTimer = (color, field, delta, currentValue, max) => {
        stopTimer();
        const valueRef = { current: currentValue };
        timerRef.current = setInterval(() => {
            const nextVal = valueRef.current + delta;
            if (nextVal >= 0 && nextVal <= max) {
                updateMatchRef.current(color, field, nextVal);
                valueRef.current = nextVal;
            }
        }, 100);
    };

    // Determine Ownership High/Low
    const redLow = matchData.red.centerLower || 0;
    const blueLow = matchData.blue.centerLower || 0;

    // In skills, ownership is manual via centerLowerOwned
    const lowOwner = isSkills ? (matchData.red.centerLowerOwned === 'red' ? 'red' : null) : (redLow > blueLow ? 'red' : (blueLow > redLow ? 'blue' : null));

    const redHigh = matchData.red.centerUpper || 0;
    const blueHigh = matchData.blue.centerUpper || 0;
    // In skills, ownership is manual via centerUpperOwned
    const highOwner = isSkills ? (matchData.red.centerUpperOwned === 'red' ? 'red' : null) : (redHigh > blueHigh ? 'red' : (blueHigh > redHigh ? 'blue' : null));


    return (
        <View
            pointerEvents="box-none"
            style={{
                paddingVertical: 0, paddingHorizontal: 10, marginBottom: -20, marginTop: -20, alignItems: 'center'
            }}
        >

            {isSkills ? (
                /* SKILLS LAYOUT: TOP/BOTTOM HORIZONTAL */
                /* SKILLS LAYOUT: TOP/BOTTOM HORIZONTAL */
                /* SKILLS LAYOUT: TOP/BOTTOM HORIZONTAL */
                <View style={{ alignItems: 'center', gap: 4 }} pointerEvents="box-none">
                    {/* TOP ROW: HIGH GOALS */}
                    <View style={{ alignItems: 'center', gap: 4, width: '100%' }} pointerEvents="box-none">
                        {/* 3-Column Layout: Spacer | Stepper | Controls */}
                        <View style={{ flexDirection: 'row', alignItems: 'center', width: '100%', zIndex: 100 }} pointerEvents="box-none">
                            {/* Left Spacer */}
                            <View style={{ flex: 1 }} />

                            {/* Center Stepper - Inline Buttons */}
                            <View style={{ flex: 1.5, alignItems: 'center', gap: 4 }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', gap: 8 }}>
                                    <ScaleButton
                                        onPress={() => updateMatch('red', 'centerUpper', Math.max(0, redHigh - 1))}
                                        onLongPress={() => startTimer('red', 'centerUpper', -1, redHigh, 7 - blueHigh)}
                                        onPressOut={() => stopTimer()}
                                        hitSlop={{ top: 15, bottom: 15, left: 20, right: 20 }}
                                        style={{
                                            width: 28, height: 28, backgroundColor: THEME.colors.error + '20', borderRadius: 8,
                                            alignItems: 'center', justifyContent: 'center',
                                            borderWidth: 2, borderColor: THEME.colors.error, elevation: 2
                                        }}
                                    >
                                        <Minus size={16} color={THEME.colors.text.primary} />
                                    </ScaleButton>

                                    <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 20, textAlign: 'center', height: 22, lineHeight: 22, includeFontPadding: false, textAlignVertical: 'center' }}>{redHigh}</Text>

                                    <ScaleButton
                                        onPress={() => updateMatch('red', 'centerUpper', Math.min(7 - blueHigh, redHigh + 1))}
                                        onLongPress={() => startTimer('red', 'centerUpper', 1, redHigh, 7 - blueHigh)}
                                        onPressOut={() => stopTimer()}
                                        hitSlop={{ top: 40, bottom: 40, left: 20, right: 20 }}
                                        style={{
                                            width: 28, height: 28, backgroundColor: THEME.colors.error, borderRadius: 8,
                                            alignItems: 'center', justifyContent: 'center',
                                            borderWidth: 2, borderColor: THEME.colors.error, elevation: 2,
                                            opacity: redHigh >= (7 - blueHigh) ? 0.3 : 1
                                        }}
                                    >
                                        <Plus size={16} color="#fff" />
                                    </ScaleButton>
                                </View>
                            </View>

                            {/* Right Controls */}
                            <View style={{ flex: 1, flexDirection: 'row', gap: 8, justifyContent: 'flex-start' }}>
                                <ScaleButton onPress={() => updateMatch('red', 'centerUpperOwned', highOwner === 'red' ? null : 'red')}>
                                    <View style={{
                                        width: 28, height: 28, borderRadius: 8,
                                        backgroundColor: highOwner === 'red' ? THEME.colors.error : 'rgba(239, 68, 68, 0.15)',
                                        borderWidth: 2, borderColor: THEME.colors.error,
                                        justifyContent: 'center', alignItems: 'center'
                                    }}>
                                        <ArrowLeft size={16} color={highOwner === 'red' ? 'white' : THEME.colors.error} />
                                    </View>
                                </ScaleButton>

                                <ScaleButton onPress={() => updateMatch('red', 'centerUpperOwned', null)}>
                                    <View style={{
                                        width: 28, height: 28, borderRadius: 14,
                                        backgroundColor: THEME.colors.surface,
                                        borderWidth: 1, borderColor: THEME.colors.text.secondary,
                                        justifyContent: 'center', alignItems: 'center'
                                    }}>
                                        <X size={14} color={THEME.colors.text.secondary} />
                                    </View>
                                </ScaleButton>
                            </View>
                        </View>
                    </View>

                    {/* CENTER IMAGE */}
                    <View style={{ width: 260, height: 180, alignItems: 'center', justifyContent: 'center', marginVertical: 0, overflow: 'hidden' }} pointerEvents="box-none">
                        <Image source={ASSETS.centerGoal} fadeDuration={100} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain' }} />
                        <Image source={ASSETS.redLow} fadeDuration={0} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain', opacity: lowOwner === 'red' ? 1 : 0 }} />
                        <Image source={ASSETS.redHigh} fadeDuration={0} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain', transform: [{ scale: 1.05 }], opacity: highOwner === 'red' ? 1 : 0 }} />
                    </View>

                    {/* BOTTOM ROW: LOW GOALS */}
                    <View style={{ alignItems: 'center', gap: 4, width: '100%' }}>
                        <Text style={{ fontSize: 12, fontWeight: 'bold', color: THEME.colors.error }}>LOW GOAL</Text>
                        {/* 3-Column Layout: Spacer | Stepper | Controls */}
                        <View style={{ flexDirection: 'row', alignItems: 'center', width: '100%', zIndex: 100 }}>
                            {/* Left Spacer */}
                            <View style={{ flex: 1 }} />

                            {/* Center Stepper - Inline Buttons */}
                            <View style={{ flex: 1.5, alignItems: 'center', gap: 4 }}>
                                <View style={{ flexDirection: 'row', alignItems: 'center', gap: 8 }}>
                                    <ScaleButton
                                        onPress={() => updateMatch('red', 'centerLower', Math.max(0, redLow - 1))}
                                        onLongPress={() => startTimer('red', 'centerLower', -1, redLow, 7 - blueLow)}
                                        onPressOut={() => stopTimer()}
                                        hitSlop={{ top: 15, bottom: 15, left: 20, right: 20 }}
                                        style={{
                                            width: 28, height: 28, backgroundColor: THEME.colors.error + '20', borderRadius: 8,
                                            alignItems: 'center', justifyContent: 'center',
                                            borderWidth: 2, borderColor: THEME.colors.error, elevation: 2
                                        }}
                                    >
                                        <Minus size={16} color={THEME.colors.text.primary} />
                                    </ScaleButton>

                                    <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 20, textAlign: 'center', height: 22, lineHeight: 22, includeFontPadding: false, textAlignVertical: 'center' }}>{redLow}</Text>

                                    <ScaleButton
                                        onPress={() => updateMatch('red', 'centerLower', Math.min(7 - blueLow, redLow + 1))}
                                        onLongPress={() => startTimer('red', 'centerLower', 1, redLow, 7 - blueLow)}
                                        onPressOut={() => stopTimer()}
                                        hitSlop={{ top: 40, bottom: 40, left: 20, right: 20 }}
                                        style={{
                                            width: 28, height: 28, backgroundColor: THEME.colors.error, borderRadius: 8,
                                            alignItems: 'center', justifyContent: 'center',
                                            borderWidth: 2, borderColor: THEME.colors.error, elevation: 2,
                                            opacity: redLow >= (7 - blueLow) ? 0.3 : 1
                                        }}
                                    >
                                        <Plus size={16} color="#fff" />
                                    </ScaleButton>
                                </View>
                            </View>

                            {/* Right Controls */}
                            <View style={{ flex: 1, flexDirection: 'row', gap: 8, justifyContent: 'flex-start' }}>
                                <ScaleButton onPress={() => updateMatch('red', 'centerLowerOwned', lowOwner === 'red' ? null : 'red')}>
                                    <View style={{
                                        width: 28, height: 28, borderRadius: 8,
                                        backgroundColor: lowOwner === 'red' ? THEME.colors.error : 'rgba(239, 68, 68, 0.15)',
                                        borderWidth: 2, borderColor: THEME.colors.error,
                                        justifyContent: 'center', alignItems: 'center'
                                    }}>
                                        <ArrowLeft size={16} color={lowOwner === 'red' ? 'white' : THEME.colors.error} />
                                    </View>
                                </ScaleButton>

                                <ScaleButton onPress={() => updateMatch('red', 'centerLowerOwned', null)}>
                                    <View style={{
                                        width: 28, height: 28, borderRadius: 14,
                                        backgroundColor: THEME.colors.surface,
                                        borderWidth: 1, borderColor: THEME.colors.text.secondary,
                                        justifyContent: 'center', alignItems: 'center'
                                    }}>
                                        <X size={14} color={THEME.colors.text.secondary} />
                                    </View>
                                </ScaleButton>
                            </View>
                        </View>
                    </View>
                </View>
            ) : (
                /* MATCH LAYOUT: SIDE-BY-SIDE VERTICAL (Previous UI) */
                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'center' }} pointerEvents="box-none">
                    {/* RED SIDE (Left Column) */}
                    <View style={{ height: 240, justifyContent: 'center', marginRight: -45, zIndex: 5 }} pointerEvents="box-none">
                        <View style={{ gap: 4 }}>
                            {/* Red High - Inline Vertical Stepper */}
                            <View style={{ alignItems: 'center', gap: 0 }}>
                                <ScaleButton
                                    onPress={() => updateMatch('red', 'centerUpper', Math.min(7 - blueHigh, redHigh + 1))}
                                    onLongPress={() => startTimer('red', 'centerUpper', 1, redHigh, 7 - blueHigh)}
                                    onPressOut={() => stopTimer()}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.error, borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 1, borderColor: THEME.colors.error, elevation: 2,
                                        opacity: redHigh >= (7 - blueHigh) ? 0.3 : 1
                                    }}
                                >
                                    <Plus size={16} color="#fff" />
                                </ScaleButton>

                                <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>{redHigh}</Text>

                                <ScaleButton
                                    onPress={() => updateMatch('red', 'centerUpper', Math.max(0, redHigh - 1))}
                                    onLongPress={() => startTimer('red', 'centerUpper', -1, redHigh, 7 - blueHigh)}
                                    onPressOut={() => stopTimer()}
                                    hitSlop={{ top: 15, bottom: 15, left: 20, right: 20 }}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.error + '20', borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 2, borderColor: THEME.colors.error, elevation: 2
                                    }}
                                >
                                    <Minus size={16} color={THEME.colors.text.primary} />
                                </ScaleButton>
                            </View>
                            {/* Red Low - Inline Vertical Stepper */}
                            <View style={{ alignItems: 'center', gap: 0 }}>
                                <ScaleButton
                                    onPress={() => updateMatch('red', 'centerLower', Math.min(7 - blueLow, redLow + 1))}
                                    onLongPress={() => startTimer('red', 'centerLower', 1, redLow, 7 - blueLow)}
                                    onPressOut={() => stopTimer()}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.error, borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 1, borderColor: THEME.colors.error, elevation: 2,
                                        opacity: redLow >= (7 - blueLow) ? 0.3 : 1
                                    }}
                                >
                                    <Plus size={16} color="#fff" />
                                </ScaleButton>

                                <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>{redLow}</Text>

                                <ScaleButton
                                    onPress={() => updateMatch('red', 'centerLower', Math.max(0, redLow - 1))}
                                    onLongPress={() => startTimer('red', 'centerLower', -1, redLow, 7 - blueLow)}
                                    onPressOut={() => stopTimer()}
                                    hitSlop={{ top: 15, bottom: 15, left: 20, right: 20 }}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.error + '20', borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 2, borderColor: THEME.colors.error, elevation: 2
                                    }}
                                >
                                    <Minus size={16} color={THEME.colors.text.primary} />
                                </ScaleButton>
                            </View>
                        </View>
                    </View>

                    {/* CENTER IMAGE (Largest) */}
                    <View style={{ width: 260, height: 180, alignItems: 'center', justifyContent: 'center', marginRight: 25, marginLeft: -5, marginTop: 0, overflow: 'hidden' }} pointerEvents="box-none">
                        <Image source={ASSETS.centerGoal} fadeDuration={100} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain' }} />
                        <Image source={ASSETS.redLow} fadeDuration={0} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain', opacity: lowOwner === 'red' ? 1 : 0 }} />
                        <Image source={ASSETS.blueLow} fadeDuration={0} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain', transform: [{ scale: 1.05 }], opacity: lowOwner === 'blue' ? 1 : 0 }} />
                        <Image source={ASSETS.redHigh} fadeDuration={0} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain', transform: [{ scale: 1.05 }], opacity: highOwner === 'red' ? 1 : 0 }} />
                        <Image source={ASSETS.blueHigh} fadeDuration={0} style={{ position: 'absolute', top: -90, left: 0, width: '100%', height: 360, resizeMode: 'contain', transform: [{ scale: 1.05 }], opacity: highOwner === 'blue' ? 1 : 0 }} />
                    </View>

                    {/* BLUE SIDE (Right Column) */}
                    <View style={{ height: 240, justifyContent: 'center', marginLeft: -45, zIndex: 5 }} pointerEvents="box-none">
                        <View style={{ gap: 4 }}>
                            {/* Blue High - Inline Vertical Stepper */}
                            <View style={{ alignItems: 'center', gap: 0 }}>
                                <ScaleButton
                                    onPress={() => updateMatch('blue', 'centerUpper', Math.min(7 - redHigh, blueHigh + 1))}
                                    onLongPress={() => startTimer('blue', 'centerUpper', 1, blueHigh, 7 - redHigh)}
                                    onPressOut={() => stopTimer()}
                                    hitSlop={{ top: 40, bottom: 40, left: 20, right: 20 }}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.blueAlliance, borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 1, borderColor: THEME.colors.blueAlliance, elevation: 2,
                                        opacity: blueHigh >= (7 - redHigh) ? 0.3 : 1
                                    }}
                                >
                                    <Plus size={16} color="#fff" />
                                </ScaleButton>

                                <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>{blueHigh}</Text>

                                <ScaleButton
                                    onPress={() => updateMatch('blue', 'centerUpper', Math.max(0, blueHigh - 1))}
                                    onLongPress={() => startTimer('blue', 'centerUpper', -1, blueHigh, 7 - redHigh)}
                                    onPressOut={() => stopTimer()}
                                    hitSlop={{ top: 15, bottom: 15, left: 20, right: 20 }}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.blueAlliance + '20', borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 2, borderColor: THEME.colors.blueAlliance, elevation: 2
                                    }}
                                >
                                    <Minus size={16} color={THEME.colors.text.primary} />
                                </ScaleButton>
                            </View>
                            {/* Blue Low - Inline Vertical Stepper */}
                            <View style={{ alignItems: 'center', gap: 0 }}>
                                <ScaleButton
                                    onPress={() => updateMatch('blue', 'centerLower', Math.min(7 - redLow, blueLow + 1))}
                                    onLongPress={() => startTimer('blue', 'centerLower', 1, blueLow, 7 - redLow)}
                                    onPressOut={() => stopTimer()}
                                    hitSlop={{ top: 40, bottom: 40, left: 20, right: 20 }}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.blueAlliance, borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 1, borderColor: THEME.colors.blueAlliance, elevation: 2,
                                        opacity: blueLow >= (7 - redLow) ? 0.3 : 1
                                    }}
                                >
                                    <Plus size={16} color="#fff" />
                                </ScaleButton>

                                <Text style={{ fontSize: 16, fontWeight: 'bold', color: THEME.colors.text.primary, minWidth: 24, textAlign: 'center', height: 20, lineHeight: 20, includeFontPadding: false, textAlignVertical: 'center', marginVertical: 4 }}>{blueLow}</Text>

                                <ScaleButton
                                    onPress={() => updateMatch('blue', 'centerLower', Math.max(0, blueLow - 1))}
                                    onLongPress={() => startTimer('blue', 'centerLower', -1, blueLow, 7 - redLow)}
                                    onPressOut={() => stopTimer()}
                                    hitSlop={{ top: 15, bottom: 15, left: 20, right: 20 }}
                                    style={{
                                        width: 28, height: 28, backgroundColor: THEME.colors.blueAlliance + '20', borderRadius: 8,
                                        alignItems: 'center', justifyContent: 'center',
                                        borderWidth: 2, borderColor: THEME.colors.blueAlliance, elevation: 2
                                    }}
                                >
                                    <Minus size={16} color={THEME.colors.text.primary} />
                                </ScaleButton>
                            </View>
                        </View>
                    </View>
                </View>
            )}
        </View>
    );
});

export default function App() {
    return (
        <ThemeProvider>
            <MainContent />
        </ThemeProvider>
    );
}
