
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
            const c = -M[k][i] / M[i][i];
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
        x[i] = (M[i][n] - sum) / M[i][i];
    }
    return x;
};

export const calculateStats = (matches, teams) => {
    if (!matches || matches.length === 0 || !teams || teams.length === 0) return {};

    // Filter for Qualification matches only and played matches
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
            bOPR[i] += parseInt(red.score);
            bDPR[i] += parseInt(blue.score);
            redTeams.forEach(id2 => {
                const j = teamIndex[id2];
                M[i][j]++;
            });
        });

        // Update M and b for Blue Alliance
        blueTeams.forEach(id1 => {
            const i = teamIndex[id1];
            bOPR[i] += parseInt(blue.score);
            bDPR[i] += parseInt(red.score);
            blueTeams.forEach(id2 => {
                const j = teamIndex[id2];
                M[i][j]++;
            });
        });
    });

    // Solve for OPR and DPR
    // Handle singular matrix cases (e.g. teams that haven't played) by adding a small regularization or checking
    // For simplicity, we'll try to solve. If it fails (NaN/Infinity), we return 0.

    // Regularization: Add small value to diagonal to ensure invertibility if needed?
    // Usually not needed if enough matches played, but good for safety.
    // Let's just try solving.

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
            ccwm: (oprs[index] || 0) - (dprs[index] || 0),
            matchesPlayed: M[index][index] // Diagonal element represents count of matches played
        };
    });

    return stats;
};
