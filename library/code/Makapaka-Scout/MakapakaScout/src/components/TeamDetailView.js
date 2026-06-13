const TeamDetailView = ({ team, apiKey, apiRequest, pushView }) => {
    return (
        <View style={styles.viewContainer}>
            <View style={{ alignItems: 'center', padding: 24 }}>
                <View style={styles.teamBadgeLarge}><Text style={styles.teamBadgeTextLarge}>{team.number}</Text></View>
                <Text style={{ fontSize: 24, fontWeight: 'bold', marginTop: 16 }}>{team.team_name}</Text>
                <Text style={{ color: 'gray' }}>{team.organization}</Text>
                <Text style={{ color: 'gray' }}>{team.location?.city}, {team.location?.region}</Text>
            </View>
            <View style={{ padding: 16 }}>
                <Text style={{ fontSize: 18, fontWeight: 'bold', marginBottom: 8 }}>Stats</Text>
                <View style={styles.card}>
                    <Text style={{ color: 'gray', textAlign: 'center' }}>Season stats coming soon...</Text>
                </View>
            </View>
        </View>
    );
};
