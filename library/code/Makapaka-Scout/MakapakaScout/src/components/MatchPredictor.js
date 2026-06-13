import React, { useState, useMemo, useEffect } from 'react';
import { View, Text, StyleSheet, Pressable, ScrollView, Modal, TextInput, FlatList } from 'react-native';
import { THEME } from '../theme';
import { useTheme } from '../context/ThemeContext';
import { ScaleButton, Stepper } from './UI';
import { Trophy, Shield, Swords, Users, Calculator, X, Search } from 'lucide-react-native';

import { calculateWinProbability } from '../utils/predictionUtils';

// Helper to calculate win probability based on score difference
// Assuming normal distribution with std dev of ~20 points
// Removed local definition in favor of shared utility

const TeamSelector = ({ label, team, onPress, color }) => {
    const { theme } = useTheme();
    const styles = useMemo(() => getStyles(theme), [theme]);
    return (
    <ScaleButton onPress={onPress} style={[styles.teamSelector, { borderColor: color }]}>
        <Text style={[styles.teamLabel, { color }]}>{label}</Text>
        <Text style={styles.teamValue}>{team ? team.number : 'Select'}</Text>
        {team && <Text style={styles.teamName} numberOfLines={1}>{team.team_name}</Text>}
    </ScaleButton>
);
};

const PredictionCard = ({ title, score, winProb, color, isWinner }) => {
    const { theme } = useTheme();
    const styles = useMemo(() => getStyles(theme), [theme]);
    return (
    <View style={[styles.predictionCard, isWinner && { borderColor: color, borderWidth: 2 }]}>
        <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', marginBottom: 8 }}>
            <Text style={[styles.cardTitle, { color }]}>{title}</Text>
            {isWinner && <Trophy size={16} color={color} />}
        </View>
        <Text style={[styles.scoreText, { color }]}>{score.toFixed(1)}</Text>
        <View style={styles.probContainer}>
            <Text style={styles.probLabel}>Win Probability</Text>
            <Text style={[styles.probValue, { color }]}>{winProb}%</Text>
        </View>
        <View style={styles.probBarBg}>
            <View style={[styles.probBarFill, { width: `${winProb}% `, backgroundColor: color }]} />
        </View>
    </View>
);
};

export const MatchPredictor = ({ teams: initialTeams, stats: initialStats, apiRequest, selectedSeason }) => {
    const { theme: THEME } = useTheme();
    const styles = useMemo(() => getStyles(THEME), [THEME]);

    // State for selected teams
    const [selections, setSelections] = useState({
        red1: null,
        red2: null,
        blue1: null,
        blue2: null
    });

    // Manual OPR overrides
    const [manualOpr, setManualOpr] = useState({});

    const [selectingFor, setSelectingFor] = useState(null); // 'red1', 'red2', 'blue1', 'blue2'
    const [search, setSearch] = useState('');
    const [searchResults, setSearchResults] = useState([]);
    const [searching, setSearching] = useState(false);

    // Search teams logic
    useEffect(() => {
        if (!search || search.length < 2) {
            setSearchResults([]);
            return;
        }

        const delayDebounceFn = setTimeout(async () => {
            setSearching(true);
            try {
                // First check local teams
                const localMatches = initialTeams.filter(t =>
                    t.number.toLowerCase().includes(search.toLowerCase())
                );

                if (localMatches.length > 0) {
                    setSearchResults(localMatches);
                } else if (apiRequest && selectedSeason) {
                    // Fetch from API
                    const data = await apiRequest('/teams', {
                        number: [search],
                        season: [selectedSeason.id],
                        my_teams: false
                    });
                    if (data && data.data) {
                        setSearchResults(data.data);
                    }
                }
            } catch (err) {
                console.log("Error searching teams:", err);
            } finally {
                setSearching(false);
            }
        }, 500);

        return () => clearTimeout(delayDebounceFn);
    }, [search, initialTeams, apiRequest, selectedSeason]);

    const handleSelect = (team) => {
        if (selectingFor) {
            setSelections(prev => ({ ...prev, [selectingFor]: team }));
            // Initialize manual OPR if not present in stats
            if (!initialStats[team.id]) {
                setManualOpr(prev => ({ ...prev, [team.id]: 0 }));
            }
            setSelectingFor(null);
            setSearch('');
        }
    };

    // Calculate predictions
    const predictions = useMemo(() => {
        const getOpr = (team) => {
            if (!team) return 0;
            if (manualOpr[team.id] !== undefined) return parseFloat(manualOpr[team.id]) || 0;
            return initialStats[team.id] ? initialStats[team.id].opr : 0;
        };

        const redScore = getOpr(selections.red1) + getOpr(selections.red2);
        const blueScore = getOpr(selections.blue1) + getOpr(selections.blue2);

        const redProb = calculateWinProbability(redScore - blueScore);
        const blueProb = 100 - redProb;

        return {
            redScore,
            blueScore,
            redProb,
            blueProb
        };
    }, [selections, initialStats, manualOpr]);

    const TeamCard = ({ selectionKey, team, color }) => (
        <View style={styles.teamCard}>
            <TeamSelector
                label={selectionKey.replace(/(\d)/, ' $1').toUpperCase()} // "red1" -> "RED 1"
                team={team}
                onPress={() => setSelectingFor(selectionKey)}
                color={color}
            />
            {team && (
                <View style={styles.oprInputContainer}>
                    <Text style={styles.oprInputLabel}>OPR:</Text>
                    <TextInput
                        style={styles.oprInput}
                        keyboardType="numeric"
                        value={String(manualOpr[team.id] !== undefined ? manualOpr[team.id] : (initialStats[team.id]?.opr?.toFixed(1) || '0'))}
                        onChangeText={(text) => setManualOpr(prev => ({ ...prev, [team.id]: text }))}
                        selectTextOnFocus
                    />
                </View>
            )}
        </View>
    );

    return (
        <ScrollView style={styles.container} contentContainerStyle={{ paddingBottom: 100 }}>
            <View style={styles.header}>
                <Swords size={24} color={THEME.colors.primary} />
                <Text style={styles.headerTitle}>Match Predictor</Text>
            </View>
            <Text style={styles.headerSubtitle}>
                Select alliances to predict match outcomes based on OPR statistics.
            </Text>

            <View style={styles.vsContainer}>
                <View style={styles.allianceGroup}>
                    <Text style={[styles.allianceHeader, { color: THEME.colors.red || '#EF4444' }]}>Red Alliance</Text>
                    <TeamCard selectionKey="red1" team={selections.red1} color={THEME.colors.red || '#EF4444'} />
                    <TeamCard selectionKey="red2" team={selections.red2} color={THEME.colors.red || '#EF4444'} />
                </View>

                <View style={styles.vsBadge}>
                    <Text style={styles.vsText}>VS</Text>
                </View>

                <View style={styles.allianceGroup}>
                    <Text style={[styles.allianceHeader, { color: THEME.colors.blueAlliance }]}>Blue Alliance</Text>
                    <TeamCard selectionKey="blue1" team={selections.blue1} color={THEME.colors.blueAlliance} />
                    <TeamCard selectionKey="blue2" team={selections.blue2} color={THEME.colors.blueAlliance} />
                </View>
            </View>

            <View style={styles.resultsContainer}>
                <PredictionCard
                    title="Red Prediction"
                    score={predictions.redScore}
                    winProb={predictions.redProb}
                    color={THEME.colors.red || '#EF4444'}
                    isWinner={predictions.redScore > predictions.blueScore}
                />

                <PredictionCard
                    title="Blue Prediction"
                    score={predictions.blueScore}
                    winProb={predictions.blueProb}
                    color={THEME.colors.blueAlliance}
                    isWinner={predictions.blueScore > predictions.redScore}
                />
            </View>

            <View style={styles.infoCard}>
                <Calculator size={16} color={THEME.colors.text.secondary} style={{ marginRight: 8 }} />
                <Text style={styles.infoText}>
                    Predictions are based on calculated Offensive Power Ratings (OPR). Actual match results may vary significantly.
                </Text>
            </View>

            {/* Team Selection Modal */}
            <Modal
                visible={!!selectingFor}
                animationType="slide"
                transparent={true}
                onRequestClose={() => setSelectingFor(null)}
            >
                <View style={styles.modalOverlay}>
                    <View style={styles.modalContent}>
                        <View style={styles.modalHeader}>
                            <Text style={styles.modalTitle}>Select Team</Text>
                            <ScaleButton onPress={() => setSelectingFor(null)}>
                                <X size={24} color={THEME.colors.text.primary} />
                            </ScaleButton>
                        </View>

                        <View style={styles.searchBar}>
                            <Search size={20} color={THEME.colors.text.secondary} />
                            <TextInput
                                style={styles.searchInput}
                                placeholder="Search team number..."
                                placeholderTextColor={THEME.colors.text.muted || THEME.colors.text.secondary}
                                value={search}
                                onChangeText={setSearch}
                                autoFocus
                            />
                        </View>

                        <FlatList
                            data={searchResults}
                            keyExtractor={item => item.id.toString()}
                            renderItem={({ item }) => (
                                <ScaleButton
                                    style={styles.teamItem}
                                    onPress={() => handleSelect(item)}
                                >
                                    <View>
                                        <Text style={styles.teamItemNumber}>{item.number}</Text>
                                        <Text style={styles.teamItemName}>{item.team_name}</Text>
                                    </View>
                                    {(initialStats[item.id] || manualOpr[item.id] !== undefined) && (
                                        <View style={styles.teamStats}>
                                            <Text style={styles.oprLabel}>OPR</Text>
                                            <Text style={styles.oprValue}>
                                                {(manualOpr[item.id] !== undefined ? manualOpr[item.id] : initialStats[item.id].opr).toFixed(1)}
                                            </Text>
                                        </View>
                                    )}
                                </ScaleButton>
                            )}
                            ListEmptyComponent={<Text style={{ padding: 20, textAlign: 'center', color: THEME.colors.text.secondary }}>{searching ? 'Searching...' : 'Search for a team'}</Text>}
                        />
                    </View>
                </View>
            </Modal>
        </ScrollView>
    );
};

const getStyles = (THEME) => StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: THEME.colors.background,
        padding: 16,
    },
    header: {
        flexDirection: 'row',
        alignItems: 'center',
        marginBottom: 8,
    },
    headerTitle: {
        fontSize: 24,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        marginLeft: 12,
        fontFamily: 'Rajdhani_600SemiBold',
    },
    headerSubtitle: {
        fontSize: 14,
        color: THEME.colors.text.secondary,
        marginBottom: 24,
        lineHeight: 20,
    },
    vsContainer: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: 32,
    },
    allianceGroup: {
        flex: 1,
        gap: 12,
    },
    allianceHeader: {
        fontSize: 16,
        fontWeight: 'bold',
        textAlign: 'center',
        marginBottom: 4,
        fontFamily: 'Rajdhani_600SemiBold',
    },
    vsBadge: {
        backgroundColor: THEME.colors.surfaceLight || '#27272A',
        paddingHorizontal: 8,
        paddingVertical: 4,
        borderRadius: 12,
        marginHorizontal: 8,
    },
    vsText: {
        color: THEME.colors.text.muted || THEME.colors.text.secondary,
        fontWeight: 'bold',
        fontSize: 12,
    },
    teamSelector: {
        backgroundColor: THEME.colors.surface,
        padding: 12,
        borderRadius: 12,
        borderWidth: 1,
        alignItems: 'center',
        height: 80,
        justifyContent: 'center',
    },
    teamLabel: {
        fontSize: 10,
        fontWeight: 'bold',
        textTransform: 'uppercase',
        marginBottom: 4,
    },
    teamValue: {
        fontSize: 18,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        fontFamily: 'Rajdhani_600SemiBold',
    },
    teamName: {
        fontSize: 10,
        color: THEME.colors.text.muted || THEME.colors.text.secondary,
        marginTop: 2,
    },
    resultsContainer: {
        gap: 16,
        marginBottom: 24,
    },
    predictionCard: {
        backgroundColor: THEME.colors.surface,
        borderRadius: 16,
        padding: 16,
        borderWidth: 1,
        borderColor: 'transparent',
    },
    cardTitle: {
        fontSize: 14,
        fontWeight: 'bold',
        textTransform: 'uppercase',
    },
    scoreText: {
        fontSize: 36,
        fontWeight: 'bold',
        fontFamily: 'Rajdhani_600SemiBold',
        marginBottom: 16,
    },
    probContainer: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: 8,
    },
    probLabel: {
        fontSize: 12,
        color: THEME.colors.text.secondary,
    },
    probValue: {
        fontSize: 14,
        fontWeight: 'bold',
    },
    probBarBg: {
        height: 6,
        backgroundColor: THEME.colors.surfaceLight || '#27272A',
        borderRadius: 3,
        overflow: 'hidden',
    },
    probBarFill: {
        height: '100%',
        borderRadius: 3,
    },
    infoCard: {
        flexDirection: 'row',
        backgroundColor: THEME.colors.surfaceLight || '#27272A',
        padding: 12,
        borderRadius: 12,
        alignItems: 'center',
    },
    infoText: {
        flex: 1,
        fontSize: 12,
        color: THEME.colors.text.secondary,
        lineHeight: 16,
    },
    // Modal Styles
    modalOverlay: {
        flex: 1,
        backgroundColor: 'rgba(0,0,0,0.5)',
        justifyContent: 'flex-end',
    },
    modalContent: {
        backgroundColor: THEME.colors.background,
        borderTopLeftRadius: 24,
        borderTopRightRadius: 24,
        height: '80%',
        padding: 16,
    },
    modalHeader: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: 16,
    },
    modalTitle: {
        fontSize: 20,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        fontFamily: 'Rajdhani_600SemiBold',
    },
    searchBar: {
        flexDirection: 'row',
        alignItems: 'center',
        backgroundColor: THEME.colors.surface,
        borderRadius: 12,
        padding: 12,
        marginBottom: 16,
    },
    searchInput: {
        flex: 1,
        marginLeft: 12,
        fontSize: 16,
        color: THEME.colors.text.primary,
    },
    teamItem: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        padding: 16,
        borderBottomWidth: 1,
        borderBottomColor: THEME.colors.surfaceLight || '#27272A',
    },
    teamItemNumber: {
        fontSize: 18,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        fontFamily: 'Rajdhani_600SemiBold',
    },
    teamItemName: {
        fontSize: 14,
        color: THEME.colors.text.secondary,
    },
    teamStats: {
        alignItems: 'flex-end',
    },
    oprLabel: {
        fontSize: 10,
        color: THEME.colors.text.muted || THEME.colors.text.secondary,
        textTransform: 'uppercase',
    },
    oprValue: {
        fontSize: 16,
        fontWeight: 'bold',
        color: THEME.colors.primary,
    },
    // New Styles
    teamCard: {
        marginBottom: 8,
    },
    oprInputContainer: {
        flexDirection: 'row',
        alignItems: 'center',
        marginTop: 4,
        justifyContent: 'center',
    },
    oprInputLabel: {
        fontSize: 12,
        color: THEME.colors.text.secondary,
        marginRight: 8,
    },
    oprInput: {
        backgroundColor: THEME.colors.surface,
        borderWidth: 1,
        borderColor: THEME.colors.border,
        borderRadius: 8,
        paddingHorizontal: 8,
        paddingVertical: 2,
        fontSize: 12,
        color: THEME.colors.text.primary,
        width: 60,
        textAlign: 'center',
    },
});
