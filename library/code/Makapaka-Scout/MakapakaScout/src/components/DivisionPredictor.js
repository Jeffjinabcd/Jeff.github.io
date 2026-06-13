import React, { useState, useMemo } from 'react';
import { View, Text, TextInput, FlatList, StyleSheet } from 'react-native';
import { ChevronRight, Users, Check } from 'lucide-react-native';
import { THEME } from '../theme';
import { useTheme } from '../context/ThemeContext';
import { ScaleButton, Stepper, AnimatedListItem } from './UI';

export const DivisionPredictor = ({ teams, onRefreshTeams }) => {
    const { theme } = useTheme();
    const styles = useMemo(() => getStyles(theme), [theme]);

    const [numDivisions, setNumDivisions] = useState(2);
    const [divisionNames, setDivisionNames] = useState({});
    const [showResults, setShowResults] = useState(false);

    // Generate division names based on count or user input
    const divisions = useMemo(() => {
        const divs = [];
        for (let i = 0; i < numDivisions; i++) {
            divs.push({
                id: i,
                name: divisionNames[i] || `Division ${i + 1}`
            });
        }
        return divs;
    }, [numDivisions, divisionNames]);

    const predictedDivisions = useMemo(() => {
        if (!teams || teams.length === 0) return [];

        // 1. Sort teams: Parse number, then sort numerically
        const sortedTeams = [...teams].sort((a, b) => {
            const numA = parseInt(a.number.replace(/\D/g, '')) || 0;
            const numB = parseInt(b.number.replace(/\D/g, '')) || 0;
            if (numA !== numB) return numA - numB;
            return a.number.localeCompare(b.number);
        });

        // 2. Distribute: Team i goes to Division (i % N)
        const distribution = divisions.map(d => ({ ...d, teams: [] }));

        sortedTeams.forEach((team, index) => {
            const divIndex = index % numDivisions;
            distribution[divIndex].teams.push(team);
        });

        return distribution;
    }, [teams, divisions, numDivisions]);

    const handleNameChange = (index, text) => {
        setDivisionNames(prev => ({ ...prev, [index]: text }));
    };

    if (!teams || teams.length === 0) {
        return (
            <View style={styles.container}>
                <View style={styles.emptyState}>
                    <Users size={48} color={theme.colors.text.secondary} />
                    <Text style={styles.emptyTitle}>Teams Required</Text>
                    <Text style={styles.emptyText}>
                        Load the team list to use the Division Predictor.
                    </Text>
                    <ScaleButton onPress={onRefreshTeams} style={styles.loadButton}>
                        <Text style={styles.loadButtonText}>Load Teams</Text>
                    </ScaleButton>
                </View>
            </View>
        );
    }

    return (
        <View style={styles.container}>
            <View style={styles.header}>
                <Text style={styles.title}>Division Predictor</Text>
                <Text style={styles.subtitle}>
                    Predict team divisions based on standard VEX round-robin distribution.
                </Text>
            </View>

            {/* Controls */}
            <View style={styles.controls}>
                <View style={styles.controlRow}>
                    <Text style={styles.label}>Number of Divisions</Text>
                    <Stepper
                        value={numDivisions}
                        onChange={(val) => {
                            setNumDivisions(val);
                            setShowResults(false);
                        }}
                        min={1}
                        max={10}
                    />
                </View>

                {/* Division Names Inputs */}
                <View style={styles.namesContainer}>
                    {divisions.map((div, i) => (
                        <View key={i} style={styles.inputRow}>
                            <Text style={styles.inputLabel}>Div {i + 1}</Text>
                            <TextInput
                                style={styles.input}
                                placeholder={`Division ${i + 1}`}
                                placeholderTextColor={theme.colors.text.secondary}
                                value={divisionNames[i]}
                                onChangeText={(text) => handleNameChange(i, text)}
                            />
                        </View>
                    ))}
                </View>

                <ScaleButton
                    onPress={() => setShowResults(!showResults)}
                    style={[styles.predictButton, showResults && styles.predictButtonActive]}
                >
                    <Text style={[styles.predictButtonText, showResults && styles.predictButtonTextActive]}>
                        {showResults ? 'Hide Prediction' : 'Predict Allocations'}
                    </Text>
                    {showResults && <Check size={18} color={theme.colors.primary} style={{ marginLeft: 8 }} />}
                </ScaleButton>
            </View>

            {/* Results */}
            {showResults && (
                <View style={styles.results}>
                    {predictedDivisions.map((div) => (
                        <View key={div.id} style={styles.divisionCard}>
                            <View style={styles.divisionHeader}>
                                <Text style={styles.divisionTitle}>{div.name}</Text>
                                <Text style={styles.divisionCount}>{div.teams.length} Teams</Text>
                            </View>
                            <View style={styles.teamGrid}>
                                {div.teams.map(team => (
                                    <View key={team.id} style={styles.teamCard}>
                                        <Text style={styles.teamNumber}>{team.number}</Text>
                                        <Text style={styles.teamName} numberOfLines={1}>
                                            {team.team_name || 'Unknown'}
                                        </Text>
                                    </View>
                                ))}
                            </View>
                        </View>
                    ))}
                </View>
            )}
        </View>
    );
};

const getStyles = (THEME) => StyleSheet.create({
    container: {
        flex: 1,
        padding: 16,
    },
    header: {
        marginBottom: 20,
    },
    title: {
        fontSize: 20,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        marginBottom: 4,
    },
    subtitle: {
        fontSize: 14,
        color: THEME.colors.text.secondary,
        lineHeight: 20,
    },
    controls: {
        backgroundColor: THEME.colors.surface,
        borderRadius: 12,
        padding: 16,
        marginBottom: 16,
        borderWidth: 1,
        borderColor: THEME.colors.border,
    },
    controlRow: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: 16,
    },
    label: {
        fontSize: 16,
        fontWeight: '600',
        color: THEME.colors.text.primary,
    },
    namesContainer: {
        gap: 8,
        marginBottom: 16,
    },
    inputRow: {
        flexDirection: 'row',
        alignItems: 'center',
        gap: 12,
    },
    inputLabel: {
        width: 40,
        fontSize: 12,
        color: THEME.colors.text.secondary,
        fontWeight: 'bold',
    },
    input: {
        flex: 1,
        backgroundColor: THEME.colors.background,
        borderRadius: 8,
        paddingHorizontal: 12,
        paddingVertical: 8,
        color: THEME.colors.text.primary,
        borderWidth: 1,
        borderColor: THEME.colors.border,
    },
    predictButton: {
        backgroundColor: THEME.colors.primary,
        borderRadius: 8,
        paddingVertical: 12,
        alignItems: 'center',
        justifyContent: 'center',
        flexDirection: 'row',
    },
    predictButtonActive: {
        backgroundColor: THEME.colors.surface,
        borderWidth: 2,
        borderColor: THEME.colors.primary,
    },
    predictButtonText: {
        color: THEME.colors.background,
        fontWeight: 'bold',
        fontSize: 16,
    },
    predictButtonTextActive: {
        color: THEME.colors.primary,
    },
    emptyState: {
        alignItems: 'center',
        justifyContent: 'center',
        padding: 32,
        backgroundColor: THEME.colors.surface,
        borderRadius: 16,
        borderWidth: 1,
        borderColor: THEME.colors.border,
        marginTop: 20,
    },
    emptyTitle: {
        fontSize: 18,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        marginTop: 16,
        marginBottom: 8,
    },
    emptyText: {
        textAlign: 'center',
        color: THEME.colors.text.secondary,
        marginBottom: 20,
    },
    loadButton: {
        backgroundColor: THEME.colors.primary,
        paddingHorizontal: 24,
        paddingVertical: 12,
        borderRadius: 8,
    },
    loadButtonText: {
        color: THEME.colors.background,
        fontWeight: 'bold',
    },
    results: {
        gap: 16,
    },
    divisionCard: {
        backgroundColor: THEME.colors.surface,
        borderRadius: 12,
        padding: 16,
        borderWidth: 1,
        borderColor: THEME.colors.border,
    },
    divisionHeader: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: 12,
        paddingBottom: 12,
        borderBottomWidth: 1,
        borderBottomColor: THEME.colors.border,
    },
    divisionTitle: {
        fontSize: 18,
        fontWeight: 'bold',
        color: THEME.colors.primary,
    },
    divisionCount: {
        fontSize: 12,
        color: THEME.colors.text.secondary,
        fontWeight: '600',
    },
    teamGrid: {
        flexDirection: 'row',
        flexWrap: 'wrap',
        gap: 8,
    },
    teamCard: {
        width: '31%', // 3 columns with gap
        aspectRatio: 1.5,
        backgroundColor: THEME.colors.background,
        padding: 8,
        borderRadius: 8,
        justifyContent: 'center',
        alignItems: 'center',
        borderWidth: 1,
        borderColor: THEME.colors.border,
    },
    teamNumber: {
        fontSize: 16,
        fontWeight: 'bold',
        color: THEME.colors.text.primary,
        marginBottom: 2,
    },
    teamName: {
        fontSize: 10,
        color: THEME.colors.text.secondary,
        textAlign: 'center',
    },
});
