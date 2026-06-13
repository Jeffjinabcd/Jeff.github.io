
import { calculateMeowScore, EVENT_TIERS } from './src/utils/meowScore.js';

// Define experimental tiers with offsets
const TIERS_WITH_OFFSET = {
    S_GOLD: { ...EVENT_TIERS.S_GOLD, offset: 15 },
    S_SILVER: { ...EVENT_TIERS.S_SILVER, offset: 5 },
    S_BRONZE: { ...EVENT_TIERS.S_BRONZE, offset: 0 },
    A_TIER: { ...EVENT_TIERS.A_TIER, offset: 0 },
    B_TIER: { ...EVENT_TIERS.B_TIER, offset: -5 },
    C_TIER: { ...EVENT_TIERS.C_TIER, offset: -15 }
};

function calculateEventScoreWithOffset(baseScore, tier) {
    // Apply Offset to Base Score ("Difficulty Adjustment")
    const effectiveBase = baseScore + (tier.offset || 0);
    
    // Center around 50
    const rawDelta = effectiveBase - 50;
    
    let multiplier = 1;
    if (rawDelta > 0) {
        multiplier = 1 + tier.swing; 
    } else {
        multiplier = 1 - tier.swing;
    }

    const scoreDelta = rawDelta * multiplier;
    return {
        baseScore,
        effectiveBase,
        final: 50 + scoreDelta
    };
}

// Scenarios
// 1. Smurf: 95 Base Score at C-Tier
const smurfBase = 95;
const smurfResult = calculateEventScoreWithOffset(smurfBase, TIERS_WITH_OFFSET.C_TIER);

// 2. Good Sig: 65 Base Score at S-Gold
const sigBase = 65;
const sigResult = calculateEventScoreWithOffset(sigBase, TIERS_WITH_OFFSET.S_GOLD);

console.log("--- Smurf vs Signature Comparison (With Offsets) ---");
console.log(`\nSmurf (C-Tier):`);
console.log(`Base: ${smurfResult.baseScore} -> Effective: ${smurfResult.effectiveBase}`);
console.log(`Final: ${smurfResult.final.toFixed(2)}`);

console.log(`\nGood Signature (S-Gold):`);
console.log(`Base: ${sigResult.baseScore} -> Effective: ${sigResult.effectiveBase}`);
console.log(`Final: ${sigResult.final.toFixed(2)}`);

console.log(`\nGap: ${(sigResult.final - smurfResult.final).toFixed(2)} points`);
