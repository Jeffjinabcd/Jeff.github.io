
// Simple linear algebra solver for OPR calculation
// Solves Ax = b using Gaussian elimination
// A is a square matrix, b is a vector

const solve = (A, b) => {
    const n = A.length;
    // Augment A with b
    const M = A.map((row, i) => [...row, b[i]]);

    // Gaussian elimination
    for (let i = 0; i < n; i++) {
        // Find pivot
        let maxEl = Math.abs(M[i][i]);
        let maxRow = i;
        for (let k = i + 1; k < n; k++) {
            if (Math.abs(M[k][i]) > maxEl) {
                maxEl = Math.abs(M[k][i]);
                maxRow = k;
            }
        }

        // Swap maximum row with current row
        [M[maxRow], M[i]] = [M[i], M[maxRow]];

        // Make all rows below this one 0 in current column
        for (let k = i + 1; k < n; k++) {
            const c = -M[k][i] / (M[i][i] || 1); // Avoid div by zero
            for (let j = i; j < n + 1; j++) {
                if (i === j) {
                    M[k][j] = 0;
                } else {
                    M[k][j] += c * M[i][j];
                }
            }
        }
    }

    // Solve equation Ax=b for an upper triangular matrix A
    const x = new Array(n).fill(0);
    for (let i = n - 1; i >= 0; i--) {
        let sum = 0;
        for (let j = i + 1; j < n; j++) {
            sum += M[i][j] * x[j];
        }
        x[i] = (M[i][n] - sum) / (M[i][i] || 1);
    }
    return x;
};

const calculateStats = (matches, teams) => {
    if (!matches || matches.length === 0 || !teams || teams.length === 0) return {};

    // Filter for Qualification matches only and played matches
    // NOTE: In Debug script, we assume mock matches are valid if round=2
    const qualMatches = matches.filter(m => m.round === 2 && m.alliances[0].score >= 0);

    if (qualMatches.length === 0) return {};

    // Map team IDs to indices
    const teamIds = teams.map(t => t.id);
    const teamIndex = {};
    teamIds.forEach((id, index) => {
        teamIndex[id] = index;
    });

    const n = teams.length;
    const M = Array(n).fill(0).map(() => Array(n).fill(0)); // Matrix M = A^T * A
    const bOPR = Array(n).fill(0); // Vector b for OPR = A^T * scores
    const bDPR = Array(n).fill(0); // Vector b for DPR = A^T * opponent_scores

    qualMatches.forEach(match => {
        const red = match.alliances.find(a => a.color === 'red');
        const blue = match.alliances.find(a => a.color === 'blue');

        if (!red || !blue) return;

        const redTeams = red.teams.map(t => t.team.id).filter(id => teamIndex[id] !== undefined);
        const blueTeams = blue.teams.map(t => t.team.id).filter(id => teamIndex[id] !== undefined);

        // Update M and b for Red Alliance
        redTeams.forEach(id1 => {
            const i = teamIndex[id1];
            bOPR[i] += red.score;
            bDPR[i] += blue.score;
            redTeams.forEach(id2 => {
                const j = teamIndex[id2];
                M[i][j]++;
            });
        });

        // Update M and b for Blue Alliance
        blueTeams.forEach(id1 => {
            const i = teamIndex[id1];
            bOPR[i] += blue.score;
            bDPR[i] += red.score;
            blueTeams.forEach(id2 => {
                const j = teamIndex[id2];
                M[i][j]++;
            });
        });
    });

    let oprs = [];
    let dprs = [];

    try {
        oprs = solve(M.map(row => [...row]), bOPR);
        dprs = solve(M.map(row => [...row]), bDPR);
    } catch (e) {
        console.log("Error solving stats:", e);
        return {};
    }

    const stats = {};
    teamIds.forEach((id, index) => {
        stats[id] = {
            opr: oprs[index] || 0,
            dpr: dprs[index] || 0,
            ccwm: (oprs[index] || 0) - (dprs[index] || 0)
        };
    });

    return stats;
};

// Mock data
const mockTeams = [
    { id: 1 },
    { id: 2 },
    { id: 3 },
    { id: 4 }
];

// Round 2 is Qualification
const mockMatches = [
    {
        round: 2,
        alliances: [
            { color: 'red', score: 20, teams: [{ team: { id: 1 } }, { team: { id: 2 } }] },
            { color: 'blue', score: 10, teams: [{ team: { id: 3 } }, { team: { id: 4 } }] }
        ]
    },
    {
        round: 2,
        alliances: [
            { color: 'red', score: 30, teams: [{ team: { id: 1 } }, { team: { id: 3 } }] },
            { color: 'blue', score: 5, teams: [{ team: { id: 2 } }, { team: { id: 4 } }] }
        ]
    }
];

console.log("Running calculateStats...");
const stats = calculateStats(mockMatches, mockTeams);
console.log("Stats Result:", JSON.stringify(stats, null, 2));
