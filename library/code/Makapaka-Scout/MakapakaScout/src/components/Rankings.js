import React, { useState, useEffect, useCallback, useRef } from 'react';
import { View, Text, FlatList, ActivityIndicator, StyleSheet, RefreshControl, TextInput } from 'react-native';
import { ScaleButton, AnimatedListItem, DropdownSelector } from './UI';
import { THEME } from '../theme';
import { useTheme } from '../context/ThemeContext';
import { Trophy, Medal, Star, Shield, Swords, TrendingUp } from 'lucide-react-native';
import { calculateMeowScore, calculateEventScore } from '../utils/meowScore';
import { calculateStats } from '../utils/stats';

const getStyles = (THEME) => StyleSheet.create({
    card: {
        flexDirection: 'row', alignItems: 'center', padding: 16, 
        backgroundColor: THEME.colors.surface, borderRadius: 12, marginBottom: 8,
        borderWidth: 1, borderColor: THEME.colors.border
    }
});

const RankingsItem = React.memo(({ item, index, activeSeason, gradeLevel, queue, getDisplayData, styles, theme }) => {
    // MeowScore calculation removed from here as per user request
    
    const { main, sub } = getDisplayData(item);

    return (
        <AnimatedListItem index={index}>
            <View style={styles.card}>
                <View style={{ width: 40, alignItems: 'center', marginRight: 12 }}>
                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: theme.colors.text.secondary }}>#{index + 1}</Text>
                    {index < 3 && <Medal size={16} color={index === 0 ? '#FFD700' : index === 1 ? '#C0C0C0' : '#CD7F32'} />}
                </View>
                <View style={{ flex: 1, paddingRight: 8 }}>
                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: theme.colors.text.primary }}>{item.team.team}</Text>
                    <Text style={{ fontSize: 12, fontWeight: 'bold', color: theme.colors.text.primary }} numberOfLines={1}>{item.team.teamName}</Text>
                    <Text style={{ fontSize: 10, color: theme.colors.text.secondary }} numberOfLines={1}>
                        {[item.team.city, item.team.region, item.team.country].filter(Boolean).join(', ')}
                    </Text>
                </View>
                <View style={{ alignItems: 'flex-end' }}>
                    <Text style={{ fontSize: 24, fontWeight: 'bold', color: theme.colors.primary }}>
                        {main}
                    </Text>
                    <Text style={{ fontSize: 10, color: theme.colors.text.secondary }}>
                        {sub}
                    </Text>
                </View>
            </View>
        </AnimatedListItem>
    );
});

// A simple queue to manage match fetching so we don't nuke the API
// This needs to be outside the component to persist across re-renders/tab switches if possible,
// or we keep it inside and cancel on unmount. Inside is safer for now.
class MatchFetchQueue {
    constructor(apiRequest, onUpdate) {
        this.queue = [];
        this.processing = false;
        this.apiRequest = apiRequest;
        this.onUpdate = onUpdate;
        this.cache = new Map(); // teamId -> stats
        this.processedCount = 0;
    }

    add(team, seasonId, grade, programmingScore = 0) {
        if (this.cache.has(team.id)) {
            // Immediately update if cached
            this.onUpdate(team.id, this.cache.get(team.id));
            return;
        }
        if (this.queue.find(t => t.id === team.id)) return; // Already queued
        
        this.queue.push({ team, seasonId, grade, programmingScore });
        this.process();
    }

    async process() {
        if (this.processing || this.queue.length === 0) return;

        this.processing = true;
        const { team, seasonId, grade, programmingScore } = this.queue.shift();

        try {
            // Fetch matches for this team
            // console.log(`[MeowQueue] Fetching matches for ${team.number}`);
            const matches = await this.apiRequest(`/teams/${team.id}/matches`, {
                season: [seasonId],
                per_page: 250 // Hope they haven't played more than 250 matches... valid for 99%
            });

            if (matches && Array.isArray(matches.data)) {
                // Calculate Stats
                // Filter for played matches (must have scores and be marked key 'scored')
                const myMatches = matches.data.filter(m => 
                    m.alliances && 
                    m.alliances[0] && 
                    m.alliances[0].score >= 0 &&
                    m.scored // IMPORTANT: Filter out unplayed/future matches
                );
                // ...
                const matchesByEvent = {};

                myMatches.forEach(m => {
                   if (!m.event || !m.event.id) return;
                   if (!matchesByEvent[m.event.id]) {
                       matchesByEvent[m.event.id] = { event: m.event, matches: [] };
                   }
                   matchesByEvent[m.event.id].matches.push(m);
                });

                const eventScores = [];

                Object.values(matchesByEvent).forEach(group => {
                    const eventMatches = group.matches;
                    
                    let eTotalScore = 0;
                    let eTotalOppScore = 0;
                    let eWins = 0;
                    let eLosses = 0;
                    let eTies = 0;
                    let eAWP = 0;
                    let eTotalWinMargin = 0;
                    
                    eventMatches.forEach(m => {
                         const isRed = m.alliances.find(a => a.color === 'red').teams.some(t => t.team.id === team.id);
                         const myAlliance = m.alliances.find(a => a.color === isRed ? 'red' : 'blue');
                         const oppAlliance = m.alliances.find(a => a.color === isRed ? 'blue' : 'red');

                         const myScore = myAlliance.score;
                         const oppScore = oppAlliance.score;
                         eTotalScore += myScore;
                         eTotalOppScore += oppScore;
                         eTotalWinMargin += (myScore - oppScore);
                         
                         let resultPoints = 0; // The WP from result (2 win, 1 tie, 0 loss)
                         if (myScore > oppScore) { eWins++; resultPoints = 2; }
                         else if (myScore < oppScore) { eLosses++; resultPoints = 0; }
                         else { eTies++; resultPoints = 1; }
                         
                         // Deducing AWP: Total WP - Result WP
                         // Assuming 'wp' property exists on alliance object (VRC)
                         const earnedWP = myAlliance.wp || 0;
                         if (earnedWP > resultPoints) { // Difference implies AWP or AP
                             // In High Stakes: AWP is explicitly the bonus. 
                             // We'll count any "Extra" WP as AWP for simplicity
                             // This handles both AWP (1pt) and potentially old Autonomous Bonus WP (rarely mixed)
                             eAWP++;
                         }
                    });

                    const eCount = eventMatches.length;
                    const eventStats = {
                        wins: eWins, losses: eLosses, ties: eTies,
                        awp: eAWP, // Calculated AWP Count
                        programming: programmingScore, // Global Prog Skills Score
                        opr: eCount > 0 ? eTotalScore / eCount : 0,
                        dpr: eCount > 0 ? eTotalOppScore / eCount : 0,
                        ccwm: eCount > 0 ? eTotalWinMargin / eCount : 0,
                        ap: 0, wp: 0, avgOpponentRating: 50,
                        isVexIQ: grade === 'Elementary' || grade === 'Middle School'
                    };

                    const { scoreDelta, tier } = calculateEventScore(eventStats, group.event);
                    // The "Event Score" contribution is the delta applied to the baseline of 50
                    eventScores.push(50 + scoreDelta);
                });

                // Overall Stats still useful for display? Or should we sum them up?
                // Let's keep the overall stats aggregation for the UI popups if needed, 
                // but use the NEW meow score.
                
                // Recalculate global aggregate for display purposes (Wins/Losses total)
                let totalWins = 0, totalLosses = 0, totalTies = 0;
                myMatches.forEach(m => {
                     const isRed = m.alliances.find(a => a.color === 'red').teams.some(t => t.team.id === team.id);
                     const myAlliance = m.alliances.find(a => a.color === isRed ? 'red' : 'blue');
                     const oppAlliance = m.alliances.find(a => a.color === isRed ? 'blue' : 'red');
                     if (myAlliance.score > oppAlliance.score) totalWins++;
                     else if (myAlliance.score < oppAlliance.score) totalLosses++;
                     else totalTies++;
                });

                // Final Score is Average of Event Scores
                const totalMeow = eventScores.reduce((a, b) => a + b, 0);
                const finalMeowScore = eventScores.length > 0 ? Math.round(totalMeow / eventScores.length) : 30; // 30 is fallback for 0 events
                
                // Create a "composite" stats object
                // Note: OPR/DPR here will be a simple average of all matches, not event-weighted
                 const compositeStats = {
                    wins: totalWins, losses: totalLosses, ties: totalTies,
                    // Simple total OPR calc
                    opr: myMatches.length > 0 ? myMatches.reduce((acc, m) => {
                         const isRed = m.alliances.find(a => a.color === 'red').teams.some(t => t.team.id === team.id);
                         return acc + m.alliances.find(a => a.color === isRed ? 'red' : 'blue').score;
                    }, 0) / myMatches.length : 0,
                    meowScore: Math.min(100, Math.max(0, finalMeowScore))
                 };

                 this.cache.set(team.id, compositeStats);
                 this.processedCount++;
                 this.onUpdate(team.id, compositeStats);
            }
        } catch (e) {
            console.error("Queue Error", e);
            // Handle Rate Limit (429) specifically
            if (e.statusCode === 429 || (e.message && (e.message.includes('429') || e.message.includes('Too Many Attempts')))) {
                console.log("Rate Limit Hit! Backing off for 10 seconds...");
                this.processing = false;
                // Re-queue the item at the front to try again
                this.queue.unshift({ team, seasonId, grade });
                setTimeout(() => this.process(), 10000); // 10s backoff for safety
                return; 
            }
        }

        this.processing = false;
        // Increase delay to 1.5s to be extremely safe
        setTimeout(() => this.process(), 1500);
    }
    stop() {
        this.queue = [];
        this.processing = false;
        // We can't easily clear the internal setTimeouts without tracking IDs, 
        // but setting queue to empty stops future processing.
    }
    
    invalidateCache() {
        this.cache.clear();
        this.processedCount = 0;
    }
}

import { COUNTRY_CODES, COUNTRIES, REGIONS } from '../data/locations';

export const Rankings = React.memo(({ apiKey, apiRequest, selectedSeason, category }) => {
    const { theme: THEME } = useTheme();
    const styles = React.useMemo(() => getStyles(THEME), [THEME]);

    // Local State for Filters & Overrides
    const [localProgramId, setLocalProgramId] = useState(selectedSeason?.program?.id || 1);
    const [activeSeason, setActiveSeason] = useState(selectedSeason);
    const [gradeLevel, setGradeLevel] = useState(category?.grade || 'High School');
    const [country, setCountry] = useState('');
    const [region, setRegion] = useState('');
    
    const [teams, setTeams] = useState([]);
    const [loading, setLoading] = useState(false);
    const [meowData, setMeowData] = useState({}); // teamId -> stats object
    
    // Queue ref
    const queueRef = useRef(null);

    // Initialize queue once
    useEffect(() => {
        queueRef.current = new MatchFetchQueue(apiRequest, (id, stats) => {
            setMeowData(prev => ({ ...prev, [id]: stats }));
        });

        return () => {
            if (queueRef.current) {
                queueRef.current.stop();
            }
        };
    }, [apiRequest]);

    // Effect: Switch Active Season when Program Changes
    useEffect(() => {
        const fetchActiveSeason = async () => {
            if (!apiKey) return;
            // If the selected program matches the global selected season's program, use it (optimization)
            if (selectedSeason?.program?.id === localProgramId) {
                setActiveSeason(selectedSeason);
                return;
            }

            try {
                // Fetch active season for the program
                const seasons = await apiRequest(`/seasons`, {
                    program: [localProgramId],
                    active: true
                });
                // Usually returns an array, pick the first/latest one
                // VEX API might return multiple active seasons? Usually just one per program.
                if (seasons && seasons.data && seasons.data.length > 0) {
                    // Sort by id desc just in case
                    const latest = seasons.data.sort((a,b) => b.id - a.id)[0];
                    setActiveSeason(latest);
                }
            } catch (e) {
                console.error("Failed to fetch active season for program", localProgramId, e);
            }
        };

        fetchActiveSeason();
    }, [localProgramId, apiKey, selectedSeason, apiRequest]);


    // Effect: Fetch Rankings when criteria changes
    useEffect(() => {
        if (!activeSeason) return;
        fetchRankings();
    }, [activeSeason, gradeLevel, country, region]);

    const fetchRankings = useCallback(async () => {
        if (!apiKey || !activeSeason) return;

        // Invalidate cache on refresh to pick up algorithm changes
        if (queueRef.current) {
            queueRef.current.invalidateCache();
        }

        setLoading(true);
        try {
            const params = {
                grade_level: gradeLevel === 'All' ? undefined : gradeLevel,
                per_page: 100,
                page: 1
            };

            if (localProgramId === 4) {
                 delete params.grade_level; // VEX U has no grade levels usually
            }

            if (!activeSeason || !activeSeason.id) {
                console.warn("No active season ID");
                setLoading(false);
                return;
            }

            const data = await apiRequest(`https://www.robotevents.com/api/seasons/${activeSeason.id}/skills`, params);
            
            if (Array.isArray(data)) {
                 let filteredData = data;
                 // Client-side filtering
                 if (country.trim()) {
                     filteredData = filteredData.filter(item => item.team && item.team.country === country.trim());
                 }
                 if (region.trim()) {
                     filteredData = filteredData.filter(item => item.team && item.team.region === region.trim());
                 }
                setTeams(filteredData);
            } else if (data && Array.isArray(data.data)) {
                 let filteredData = data.data;
                 if (country.trim()) {
                     filteredData = filteredData.filter(item => item.team && item.team.country === country.trim());
                 }
                 if (region.trim()) {
                     filteredData = filteredData.filter(item => item.team && item.team.region === region.trim());
                 }
                setTeams(filteredData);
            } else {
                setTeams([]);
            }
        } catch (e) {
            console.error("Rankings Fetch Error", e);
            setTeams([]);
        } finally {
            setLoading(false);
        }
    }, [apiKey, activeSeason, gradeLevel, country, region, localProgramId, apiRequest]);

    // ... existing meow mode placeholders ...

    const getDisplayData = (item) => {
        // ... existing getDisplayData implementation ...
        const score = item.scores ? item.scores.score : item.score;
        const driver = item.scores ? item.scores.driver : item.driver_score;
        const programming = item.scores ? item.scores.programming : item.programming_score;
        return {
            main: score,
            sub: `Driver: ${driver} | Prog: ${programming}`
        };
    };

    const displayList = teams; // Just raw teams for now

    // ... existing Filter UI Helpers ...
    const ProgramTab = ({ label, id }) => (
        <ScaleButton 
            onPress={() => {
                setLocalProgramId(id);
                // Reset grade default when switching to IQ
                if (id === 41) setGradeLevel('Middle School');
                else if (id === 1) setGradeLevel('High School');
            }}
            style={{ 
                flex: 1, padding: 8, alignItems: 'center', borderRadius: 8,
                backgroundColor: localProgramId === id ? THEME.colors.primary : THEME.colors.surface,
                borderWidth: 1, borderColor: localProgramId === id ? THEME.colors.primary : THEME.colors.border
            }}
        >
            <Text style={{ fontWeight: 'bold', fontSize: 12, color: localProgramId === id ? 'black' : THEME.colors.text.secondary }}>{label}</Text>
        </ScaleButton>
    );

    const GradeTab = ({ label, value }) => (
        <ScaleButton 
            onPress={() => setGradeLevel(value)}
            style={{ 
                paddingHorizontal: 12, paddingVertical: 6, borderRadius: 16,
                backgroundColor: gradeLevel === value ? THEME.colors.primary : THEME.colors.surface,
                borderWidth: 1, borderColor: gradeLevel === value ? THEME.colors.primary : THEME.colors.border
            }}
        >
            <Text style={{ fontWeight: 'bold', fontSize: 12, color: gradeLevel === value ? 'black' : THEME.colors.text.secondary }}>{label}</Text>
        </ScaleButton>
    );

    return (
        <View style={{ flex: 1, backgroundColor: THEME.colors.background }}>
            <View style={{ padding: 12, gap: 12, backgroundColor: THEME.colors.surface, borderBottomWidth: 1, borderBottomColor: THEME.colors.border }}>
                {/* ... existing header ... */} 
                {/* (Keeping existing header code, only changing FlatList below) */} 
                <View style={{ flexDirection: 'row', gap: 8 }}>
                    <ProgramTab label="VRC" id={1} />
                    <ProgramTab label="VEX IQ" id={41} />
                    <ProgramTab label="VEX U" id={4} />
                </View>

                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between' }}>
                     <View style={{ flexDirection: 'row', gap: 8 }}>
                        {localProgramId === 1 && (
                            <>
                                <GradeTab label="High School" value="High School" />
                                <GradeTab label="Middle School" value="Middle School" />
                            </>
                        )}
                        {localProgramId === 41 && (
                            <>
                                <GradeTab label="Middle School" value="Middle School" />
                                <GradeTab label="Elementary" value="Elementary" />
                            </>
                        )}
                        {localProgramId === 4 && (
                            <Text style={{ fontSize: 12, color: THEME.colors.text.secondary, fontStyle: 'italic' }}>College / University</Text>
                        )}
                     </View>
                </View>

                 <View style={{ flexDirection: 'row', gap: 8, zIndex: 10 }}>
                    <View style={{ flex: 1 }}>
                         <DropdownSelector
                            label="Country"
                            placeholder="All Countries"
                            value={country}
                            options={[{label: 'Clear Filter', value: ''}, ...COUNTRIES]}
                            onSelect={(val) => {
                                setCountry(val);
                                setRegion(''); 
                            }}
                         />
                    </View>
                    <View style={{ flex: 1 }}>
                         <DropdownSelector
                            label="Region"
                            placeholder="All Regions"
                            value={region}
                            options={[{label: 'Clear Filter', value: ''}, ...(country && REGIONS[country] ? REGIONS[country] : [])]}
                            onSelect={setRegion}
                            hasSearch={true}
                         />
                    </View>
                 </View>
                 
                 {activeSeason && (
                     <Text style={{ fontSize: 10, color: THEME.colors.text.secondary, textAlign: 'center' }}>
                         Viewing: {activeSeason.name}
                     </Text>
                 )}
            </View>


            <FlatList
                data={displayList}
                keyExtractor={item => item.team.id.toString()}
                contentContainerStyle={{ padding: 16, paddingBottom: 100 }}
                refreshControl={<RefreshControl refreshing={loading} onRefresh={fetchRankings} />}
                renderItem={({ item, index }) => (
                    <RankingsItem 
                        item={item} 
                        index={index} 
                        activeSeason={activeSeason}
                        gradeLevel={gradeLevel}
                        queue={queueRef.current}
                        getDisplayData={getDisplayData}
                        meowData={meowData}
                        styles={styles}
                        theme={THEME}
                    />
                )}
                ListEmptyComponent={
                    <View style={{ padding: 20, alignItems: 'center', marginTop: 50 }}>
                        <Trophy size={48} color={THEME.colors.text.secondary} style={{ opacity: 0.5 }} />
                        <Text style={{ marginTop: 16, color: THEME.colors.text.secondary, fontSize: 16 }}>
                            {loading ? "Loading rankings..." : "No rankings found."}
                        </Text>
                        <Text style={{ marginTop: 8, color: 'gray', fontSize: 12, textAlign: 'center' }}>
                            Try adjusting your filters.
                        </Text>
                    </View>
                }
            />
        </View>
    );
});

