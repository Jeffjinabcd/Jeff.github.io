
import { calculateEventScore, EVENT_TIERS } from './src/utils/meowScore.js';

// Mock Stats - identical performance
const mockStats = {
    wins: 8, losses: 2, ties: 0,
    opr: 60, dpr: 10, ccwm: 50,
    ap: 4, wp: 2, avgOpponentRating: 50
};

// Mock Events
const eventS = { name: 'Kala Signature Event', level: 'Signature' }; // Should be S-Gold (Swing 0.5)
const eventC = { name: 'Local Scrimmage', event_type: 'Scrimmage' }; // Should be C-Tier (Swing -0.5)

console.log("--- Verifying Swing Isolation ---");

// 1. Calculate S-Tier
const scoreS = calculateEventScore(mockStats, eventS);
console.log(`\nEvent 1 (S-Tier):`);
console.log(`Base Score: ${scoreS.baseScore}`);
console.log(`Tier: ${scoreS.tier.name} (Swing: ${scoreS.tier.swing})`);
console.log(`Score Delta: ${scoreS.scoreDelta}`);
console.log(`Final Event Score: ${50 + scoreS.scoreDelta}`);

// 2. Calculate C-Tier
const scoreC = calculateEventScore(mockStats, eventC);
console.log(`\nEvent 2 (C-Tier):`);
console.log(`Base Score: ${scoreC.baseScore}`);
console.log(`Tier: ${scoreC.tier.name} (Swing: ${scoreC.tier.swing})`);
console.log(`Score Delta: ${scoreC.scoreDelta}`);
console.log(`Final Event Score: ${50 + scoreC.scoreDelta}`);

// 3. Check for bleed (Modify stats for one, check the other?)
// The function is pure, so bleed is impossible unless the object is mutated.
// But let's verify different stats = different scores independently.

const mockStatsBad = { ...mockStats, wins: 2, losses: 8 };
const scoreMix = calculateEventScore(mockStatsBad, eventS);
console.log(`\nEvent 3 (S-Tier, Bad Performance):`);
console.log(`Base Score: ${scoreMix.baseScore}`);
console.log(`Tier: ${scoreMix.tier.name} (Swing: ${scoreMix.tier.swing})`);
console.log(`Score Delta: ${scoreMix.scoreDelta}`);
console.log(`Final Event Score: ${50 + scoreMix.scoreDelta}`);

console.log("\nIf S-Tier score (Event 1) is DIFFERENT from C-Tier score (Event 2) for SAME stats, then tiers are working properly per event.");
