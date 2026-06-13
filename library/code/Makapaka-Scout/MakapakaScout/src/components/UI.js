import React, { useRef, useEffect } from 'react';
import { View, Text, TouchableOpacity, StyleSheet, Platform, Animated, Pressable, Easing, PanResponder, Dimensions, Modal, FlatList, TextInput } from 'react-native';
import { SafeAreaView } from 'react-native-safe-area-context';
import { ChevronLeft, Minus, Plus, X, Search, Check, ChevronDown } from 'lucide-react-native';
import { BlurView } from 'expo-blur';
import { THEME } from '../theme';
import { useTheme } from '../context/ThemeContext';

const AnimatedPressable = Animated.createAnimatedComponent(Pressable);

export const ScaleButton = ({ onPress, style, children, activeScale = 0.95, ...props }) => {
    const scale = useRef(new Animated.Value(1)).current;

    const handlePressIn = (e) => {
        // Debug reporting
        if (typeof global.reportClick === 'function') {
            global.reportClick(e.nativeEvent.pageX, e.nativeEvent.pageY, true);
        }
        Animated.spring(scale, {
            toValue: activeScale,
            useNativeDriver: true,
            speed: 50,
            bounciness: 10,
        }).start();
        if (props.onPressIn) props.onPressIn(e);
    };

    const handlePressOut = (e) => {
        Animated.spring(scale, {
            toValue: 1,
            useNativeDriver: true,
            speed: 50,
            bounciness: 10,
        }).start();
        if (props.onPressOut) props.onPressOut(e);
    };

    return (
        <AnimatedPressable
            hitSlop={props.hitSlop || { top: 40, bottom: 40, left: 40, right: 40 }}
            onPress={onPress}
            {...props}
            onPressIn={handlePressIn}
            onPressOut={handlePressOut}
            style={[{ transform: [{ scale }] }, style]}
        >
            {children}
        </AnimatedPressable>
    );
};

export const TabFadeView = ({ active, children, style }) => {
    const fadeAnim = useRef(new Animated.Value(active ? 1 : 0)).current;
    const translateY = useRef(new Animated.Value(active ? 0 : 10)).current;
    const scale = useRef(new Animated.Value(active ? 1 : 0.98)).current;
    const [isHidden, setIsHidden] = React.useState(!active);

    useEffect(() => {
        if (active) {
            setIsHidden(false);
            // Enter animation
            Animated.parallel([
                Animated.timing(fadeAnim, {
                    toValue: 1,
                    duration: 200, // Reduced from 250 for snappier feel
                    useNativeDriver: true,
                    easing: Easing.out(Easing.cubic),
                }),
                Animated.timing(translateY, {
                    toValue: 0,
                    duration: 200,
                    useNativeDriver: true,
                    easing: Easing.out(Easing.cubic),
                }),
                Animated.timing(scale, {
                    toValue: 1,
                    duration: 200,
                    useNativeDriver: true,
                    easing: Easing.out(Easing.cubic),
                }),
            ]).start();
        } else {
            // Exit animation
            Animated.parallel([
                Animated.timing(fadeAnim, {
                    toValue: 0,
                    duration: 150, // Reduced from 200
                    useNativeDriver: true,
                    easing: Easing.in(Easing.cubic),
                }),
                Animated.timing(translateY, {
                    toValue: 10,
                    duration: 150,
                    useNativeDriver: true,
                    easing: Easing.in(Easing.cubic),
                }),
                Animated.timing(scale, {
                    toValue: 0.98,
                    duration: 150,
                    useNativeDriver: true,
                    easing: Easing.in(Easing.cubic),
                }),
            ]).start(({ finished }) => {
                if (finished) {
                    setIsHidden(true);
                }
            });
        }
    }, [active]);

    return (
        <Animated.View
            style={[
                {
                    opacity: fadeAnim,
                    transform: [{ translateY }, { scale }],
                    position: 'absolute',
                    top: 0,
                    left: 0,
                    right: 0,
                    bottom: 0,
                    zIndex: active ? 1 : 0,
                    display: isHidden ? 'none' : 'flex'
                },
                style
            ]}
            pointerEvents={active ? 'auto' : 'none'}
        >
            {children}
        </Animated.View>
    );
};

export const ScreenTransition = ({ children, style, visible = true, onExited, onSwipeBack }) => {
    const { theme } = useTheme(); // Fix: get theme from context
    const fadeAnim = useRef(new Animated.Value(0)).current;
    const translateY = useRef(new Animated.Value(20)).current;
    const translateX = useRef(new Animated.Value(0)).current;

    const panResponder = useRef(
        PanResponder.create({
            onMoveShouldSetPanResponder: (evt, gestureState) => {
                // Only enable if onSwipeBack is provided and swipe starts from left half of screen
                return !!onSwipeBack && gestureState.dx > 10 && Math.abs(gestureState.dy) < gestureState.dx;
            },
            onPanResponderMove: (evt, gestureState) => {
                if (gestureState.dx > 0) {
                    translateX.setValue(gestureState.dx);
                }
            },
            onPanResponderRelease: (evt, gestureState) => {
                // Trigger: Dragged far enough (>100) OR flicked fast enough (>1.0) with some movement (>10)
                if (gestureState.dx > 100 || (gestureState.dx > 10 && gestureState.vx > 1.0)) {
                    // Start animation immediately to prevent lag while state updates
                    Animated.spring(translateX, {
                        toValue: Dimensions.get('window').width,
                        useNativeDriver: true,
                        speed: 14,
                        bounciness: 2,
                        velocity: gestureState.vx, // Maintain momentum
                        overshootClamping: true
                    }).start();

                    if (onSwipeBack) onSwipeBack();
                } else {
                    // Reset
                    Animated.spring(translateX, {
                        toValue: 0,
                        useNativeDriver: true,
                    }).start();
                }
            },
        })
    ).current;

    useEffect(() => {
        if (visible) {
            // Enter animation
            Animated.parallel([
                Animated.timing(fadeAnim, {
                    toValue: 1,
                    duration: 200,
                    useNativeDriver: true,
                    easing: Easing.out(Easing.cubic),
                }),
                Animated.timing(translateY, {
                    toValue: 0,
                    duration: 200,
                    useNativeDriver: true,
                    easing: Easing.out(Easing.cubic),
                }),
                Animated.timing(translateX, {
                    toValue: 0,
                    duration: 200,
                    useNativeDriver: true,
                    easing: Easing.out(Easing.cubic),
                }),
            ]).start();
        } else {
            // Exit animation
            Animated.parallel([
                Animated.timing(fadeAnim, {
                    toValue: 0,
                    duration: 200,
                    useNativeDriver: true,
                }),
                Animated.spring(translateX, {
                    toValue: Dimensions.get('window').width,
                    useNativeDriver: true,
                    speed: 14,
                    bounciness: 2,
                    overshootClamping: true
                }),
            ]).start(({ finished }) => {
                if (finished && onExited) {
                    onExited();
                }
            });
        }
    }, [visible]);

    return (
        <Animated.View
            style={[
                {
                    flex: 1,
                    opacity: fadeAnim,
                    transform: [{ translateY }, { translateX }],
                    backgroundColor: theme.colors.background,
                    shadowColor: '#000',
                    shadowOffset: { width: -2, height: 0 },
                    shadowOpacity: 0.1,
                    shadowRadius: 5,
                    elevation: 5
                },
                style
            ]}
            {...(onSwipeBack ? panResponder.panHandlers : {})}
        >
            {children}
        </Animated.View>
    );
};

export const Header = ({ title, showBack, onBack, rightElement, subtitle }) => {
    const { theme } = useTheme();
    const styles = React.useMemo(() => getStyles(theme), [theme]);
    return (
        <SafeAreaView edges={['top']} style={styles.headerContainer}>
            <View style={styles.headerContent}>
                <View style={styles.headerLeft}>
                    {showBack && (
                        <ScaleButton onPress={onBack} style={styles.backButton} hitSlop={{ top: 20, bottom: 20, left: 20, right: 20 }}>
                            <View pointerEvents="none">
                                <ChevronLeft size={28} color={theme.colors.primary} />
                            </View>
                        </ScaleButton>
                    )}
                    <View>
                        <Text style={styles.headerTitle} numberOfLines={1}>{title}</Text>
                        {subtitle && <Text style={styles.headerSubtitle}>{subtitle}</Text>}
                    </View>
                </View>
                <View style={styles.headerRight}>{rightElement}</View>
            </View>
        </SafeAreaView>
    );
};

export const TabBar = ({ activeTab, onTabChange, tabs }) => {
    const { theme } = useTheme();
    const styles = React.useMemo(() => getStyles(theme), [theme]);
    return (
        <View style={styles.tabBarContainer}>
            <View style={styles.tabBarShadow}>
                <View style={styles.tabBarOverflow}>
                    <BlurView intensity={50} tint="systemThinMaterialDark" style={styles.tabBarContent}>
                        {tabs.map((tab) => {
                            const Icon = tab.icon;
                            const isActive = activeTab === tab.id;
                            return (
                                <ScaleButton
                                    key={tab.id}
                                    onPress={() => onTabChange(tab.id)}
                                    style={styles.tabButton}
                                    activeScale={0.9}
                                    hitSlop={{ top: 10, bottom: 10, left: 10, right: 10 }}
                                >
                                    <View style={{ alignItems: 'center' }} pointerEvents="none">
                                        <View style={[styles.iconContainer, isActive && styles.iconContainerActive]}>
                                            <Icon
                                                size={24}
                                                color={isActive ? theme.colors.primary : '#9CA3AF'}
                                                strokeWidth={isActive ? 2.5 : 2}
                                            />
                                        </View>
                                        <Text style={[styles.tabLabel, isActive && styles.tabLabelActive]} numberOfLines={1}>{tab.label}</Text>
                                    </View>
                                </ScaleButton>
                            );
                        })}
                    </BlurView>
                </View>
            </View>
        </View>
    );
};


export const Stepper = ({ value, onChange, min = 0, max = 999, activeColor, style }) => {
    const { theme } = useTheme();
    const styles = React.useMemo(() => getStyles(theme), [theme]);
    return (
        <View style={[styles.stepperContainer, style]}>
            <ScaleButton
                onPress={() => onChange(Math.max(min, value - 1))}
                style={[styles.stepperButton, { width: 28, height: 28, borderRadius: 8 }, activeColor && { borderColor: activeColor, backgroundColor: activeColor + '15' }]}
            >
                <Minus size={16} color={activeColor || theme.colors.text.primary} />
            </ScaleButton>
            <Text style={[styles.stepperValue, { fontSize: 16, width: 28 }]}>{value}</Text>
            <ScaleButton
                onPress={() => onChange(Math.min(max, value + 1))}
                style={[
                    styles.stepperButton,
                    styles.stepperButtonActive,
                    { width: 28, height: 28, borderRadius: 8 },
                    activeColor && { backgroundColor: activeColor, borderColor: activeColor },
                    { opacity: value >= max ? 0.3 : 1 }
                ]}
            >
                <Plus size={16} color={activeColor ? '#fff' : theme.colors.text.base} />
            </ScaleButton>
        </View>
    );
};

export const AnimatedListItem = ({ children, index = 0, delay = 50 }) => {
    const fadeAnim = useRef(new Animated.Value(0)).current;
    const translateY = useRef(new Animated.Value(20)).current;

    useEffect(() => {
        Animated.parallel([
            Animated.timing(fadeAnim, {
                toValue: 1,
                delay: index * delay,
                duration: 300,
                useNativeDriver: true,
                easing: Easing.out(Easing.cubic),
            }),
            Animated.timing(translateY, {
                toValue: 0,
                delay: index * delay,
                duration: 300,
                useNativeDriver: true,
                easing: Easing.out(Easing.cubic),
            }),
        ]).start();
    }, [index]);

    return (
        <Animated.View
            style={{
                opacity: fadeAnim,
                transform: [{ translateY }],
            }}
        >
            {children}
        </Animated.View>
    );
};
export const DropdownSelector = ({ label, value, options, onSelect, placeholder = "Select...", searchPlaceholder = "Search...", hasSearch = true }) => {
    const { theme } = useTheme();
    const styles = React.useMemo(() => getStyles(theme), [theme]);
    const [visible, setVisible] = React.useState(false);
    const [search, setSearch] = React.useState('');

    const filteredOptions = React.useMemo(() => {
        if (!search) return options;
        return options.filter(opt => {
            const label = typeof opt === 'string' ? opt : opt.label;
            return label.toLowerCase().includes(search.toLowerCase());
        });
    }, [options, search]);

    const getLabel = () => {
        if (!value) return placeholder;
        const selected = options.find(opt => (typeof opt === 'string' ? opt : opt.value) === value);
        if (selected) return typeof selected === 'string' ? selected : selected.label;
        return value; // Fallback to value if not in options (e.g. custom)
    };

    return (
        <>
            <ScaleButton
                onPress={() => {
                    setSearch('');
                    setVisible(true);
                }}
                style={styles.dropdownButton}
            >
                <View style={{ flex: 1 }}>
                    {label && <Text style={styles.dropdownLabel}>{label}</Text>}
                    <Text style={[styles.dropdownValue, !value && { color: theme.colors.text.secondary }]}>
                        {getLabel()}
                    </Text>
                </View>
                <ChevronDown size={20} color={theme.colors.text.secondary} />
            </ScaleButton>

            <Modal
                visible={visible}
                transparent={true}
                animationType="slide"
                onRequestClose={() => setVisible(false)}
            >
                <View style={styles.modalOverlay}>
                    <View style={styles.modalContent}>
                        <View style={styles.modalHeader}>
                            <Text style={styles.modalTitle}>{label || placeholder}</Text>
                            <ScaleButton onPress={() => setVisible(false)} style={styles.closeButton}>
                                <X size={24} color={theme.colors.text.secondary} />
                            </ScaleButton>
                        </View>

                        {hasSearch && (
                            <View style={styles.searchContainer}>
                                <Search size={20} color={theme.colors.text.secondary} />
                                <TextInput
                                    style={styles.modalSearchInput}
                                    placeholder={searchPlaceholder}
                                    placeholderTextColor={theme.colors.text.secondary}
                                    value={search}
                                    onChangeText={setSearch}
                                    autoFocus={false} // Avoid keyboard jumping on mount sometimes
                                />
                                {search.length > 0 && (
                                    <ScaleButton onPress={() => setSearch('')}>
                                        <View style={{ backgroundColor: theme.colors.text.secondary, borderRadius: 10, padding: 2 }}>
                                            <X size={12} color={theme.colors.background} />
                                        </View>
                                    </ScaleButton>
                                )}
                            </View>
                        )}

                        <FlatList
                            data={filteredOptions}
                            keyExtractor={(item, index) => {
                                const val = typeof item === 'string' ? item : item.value;
                                return `${val}-${index}`;
                            }}
                            renderItem={({ item }) => {
                                const itemValue = typeof item === 'string' ? item : item.value;
                                const itemLabel = typeof item === 'string' ? item : item.label;
                                const isSelected = itemValue === value;

                                return (
                                    <ScaleButton
                                        style={[styles.optionItem, isSelected && styles.optionItemSelected]}
                                        onPress={() => {
                                            onSelect(itemValue);
                                            setVisible(false);
                                        }}
                                    >
                                        <Text style={[styles.optionText, isSelected && styles.optionTextSelected]}>
                                            {itemLabel}
                                        </Text>
                                        {isSelected && <Check size={20} color={theme.colors.primary} />}
                                    </ScaleButton>
                                );
                            }}
                            contentContainerStyle={{ padding: 16 }}
                            keyboardShouldPersistTaps="handled"
                        />
                    </View>
                </View>
            </Modal>
        </>
    );
};
const getStyles = (THEME) => StyleSheet.create({
    headerContainer: {
        backgroundColor: THEME.colors.background,
        borderBottomWidth: 1,
        borderBottomColor: THEME.colors.border,
        zIndex: 20,
    },
    headerContent: {
        flexDirection: 'row',
        alignItems: 'center',
        justifyContent: 'space-between',
        paddingHorizontal: 16,
        paddingBottom: 12,
        paddingTop: 8,
        minHeight: 50,
    },
    headerLeft: {
        flexDirection: 'row',
        alignItems: 'center',
        flex: 1,
    },
    backButton: {
        padding: 4,
        marginLeft: -8,
        marginRight: 4,
    },
    headerTitle: {
        ...THEME.typography.header,
        fontSize: 20,
        color: THEME.colors.text.primary,
    },
    headerSubtitle: {
        fontSize: 12,
        color: THEME.colors.text.secondary,
        marginTop: 2,
        textTransform: 'uppercase',
        letterSpacing: 1,
    },
    headerRight: {
        marginLeft: 8,
    },
    tabBarContainer: {
        position: 'absolute',
        bottom: 15,
        left: 20,
        right: 20,
        zIndex: 100,
        height: 70,
    },
    tabBarShadow: {
        shadowColor: '#000',
        shadowOffset: { width: 0, height: 10 },
        shadowOpacity: 0.5, // Darker shadow for dark mode
        shadowRadius: 20,
        elevation: 10,
        borderRadius: 35,
    },
    tabBarOverflow: {
        borderRadius: 35,
        overflow: 'hidden',
        borderWidth: 1,
        borderColor: 'rgba(255,255,255,0.1)', // Subtle light border for dark glass
    },
    tabBarContent: {
        flexDirection: 'row',
        justifyContent: 'space-around',
        alignItems: 'center',
        paddingVertical: 10,
        height: 70,
        backgroundColor: 'rgba(30,30,30,0.3)', // More transparent dark fallback
    },
    tabButton: {
        flex: 1,
        alignItems: 'center',
        justifyContent: 'center',
        height: '100%',
    },
    iconContainer: {
        padding: 8,
        borderRadius: 12,
    },
    iconContainerActive: {
        backgroundColor: 'rgba(59, 130, 246, 0.2)', // Slightly stronger highlight
    },
    tabLabel: {
        fontSize: 9,
        fontWeight: '600',
        marginTop: 2,
        color: '#9CA3AF', // Light gray for inactive text on dark bg
    },
    tabLabelActive: {
        color: THEME.colors.primary,
    },
    stepperContainer: {
        flexDirection: 'row',
        alignItems: 'center',
        gap: 8,
    },
    stepperButton: {
        width: 24,
        height: 24,
        backgroundColor: THEME.colors.surface,
        borderRadius: 4, // Sharper corners
        alignItems: 'center',
        justifyContent: 'center',
        borderWidth: 1,
        borderColor: THEME.colors.border,
    },
    stepperButtonActive: {
        backgroundColor: THEME.colors.primary,
        borderColor: THEME.colors.primary,
    },
    stepperValue: {
        fontSize: 14,
        fontWeight: 'bold',
        width: 24,
        textAlign: 'center',
        fontVariant: ['tabular-nums'],
        color: THEME.colors.text.primary,
    },

    // Dropdown
    dropdownButton: {
        flexDirection: 'row',
        alignItems: 'center',
        justifyContent: 'space-between',
        backgroundColor: THEME.colors.surface,
        borderWidth: 1,
        borderColor: THEME.colors.border,
        borderRadius: 8,
        padding: 12, // Increased padding
        minHeight: 48,
    },
    dropdownLabel: {
        fontSize: 10,
        color: THEME.colors.text.secondary,
        textTransform: 'uppercase',
        marginBottom: 2,
    },
    dropdownValue: {
        fontSize: 14,
        fontWeight: '500',
        color: THEME.colors.text.primary,
    },
    modalOverlay: {
        flex: 1,
        backgroundColor: 'rgba(0,0,0,0.5)',
        justifyContent: 'flex-end',
    },
    modalContent: {
        backgroundColor: THEME.colors.surface,
        borderTopLeftRadius: 20,
        borderTopRightRadius: 20,
        maxHeight: '80%',
        paddingBottom: 20, // Safe area padding
    },
    modalHeader: {
        flexDirection: 'row',
        alignItems: 'center',
        justifyContent: 'space-between',
        padding: 16,
        borderBottomWidth: 1,
        borderBottomColor: THEME.colors.border,
    },
    modalTitle: {
        fontSize: 18,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
    },
    closeButton: {
        padding: 4,
    },
    searchContainer: {
        flexDirection: 'row',
        alignItems: 'center',
        backgroundColor: THEME.colors.background,
        margin: 16,
        marginBottom: 0,
        paddingHorizontal: 12,
        borderRadius: 12,
        height: 44,
        gap: 8,
    },
    modalSearchInput: {
        flex: 1,
        fontSize: 16,
        color: THEME.colors.text.primary,
        height: '100%',
    },
    optionItem: {
        flexDirection: 'row',
        alignItems: 'center',
        justifyContent: 'space-between',
        paddingVertical: 14,
        paddingHorizontal: 12,
        borderBottomWidth: 1,
        borderBottomColor: THEME.colors.border,
    },
    optionItemSelected: {
        backgroundColor: 'rgba(59, 130, 246, 0.1)', // Subtle blue highlight
    },
    optionText: {
        fontSize: 16,
        color: THEME.colors.text.primary,
    },
    optionTextSelected: {
        color: THEME.colors.primary,
        fontWeight: '600',
    },
});
