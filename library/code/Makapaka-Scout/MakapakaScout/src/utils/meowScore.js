/**
 * Meow Score - Team Strength Rating Algorithm
 * 
 * Calculates a comprehensive 0-100 rating for teams based on:
 * - Win Performance (23 points)
 * - OPR - Offensive Power (18 points)
 * - DPR - Defensive Power (18 points, inverted)
 * - CCWM - Win Margin (17 points)
 * - AP/WP - Ranking Points (12 points)
 * - Strength of Schedule (12 points)
 */

/**
 * Event Tiers and Swing Factors
 */
export const EVENT_TIERS = {
    S_GOLD: { name: 'S-Gold', swing: 0.5, offset: 15, color: '#FFD700' },     // High stakes (e.g. Kala, Sugar Rush)
    S_SILVER: { name: 'S-Silver', swing: 0.25, offset: 5, color: '#C0C0C0' }, // Mid-high stakes
    S_BRONZE: { name: 'S-Bronze', swing: 0.1, offset: 0, color: '#CD7F32' },  // Lower stakes signatures
    A_TIER: { name: 'A-Tier', swing: 0.0, offset: 0, color: '#A52A2A' },      // Standard strong events
    B_TIER: { name: 'B-Tier', swing: -0.5, offset: -10, color: '#4682B4' },    // Standard Qualifiers: Dampened upside (Max ~77)
    C_TIER: { name: 'C-Tier', swing: -0.8, offset: -30, color: '#8FBC8F' }    // Scrims/Practice: Heavily capped (Max ~60)
};

/**
 * Determine Event Tier based on event properties
 * @param {Object} event - Event object
 * @returns {Object} Tier object from EVENT_TIERS
 */
export function getEventTier(event) {
    if (!event) return EVENT_TIERS.B_TIER; 
    
    // Normalize properties
    // RobotEvents API v2 usually has 'level' and 'event_type'
    const name = (event.name || '').toLowerCase();
    const level = (event.level || '').toLowerCase();
    const type = (event.event_type || '').toLowerCase();

    // S-Tier: Signature Events & Worlds
    // User specific overrides for specific signatures still require name check
    if (level === 'signature' || level === 'world' || name.includes('signature')) {
        if (name.includes('sugar rush') || name.includes('kala')) return EVENT_TIERS.S_GOLD;
        if (name.includes('haunted')) return EVENT_TIERS.S_BRONZE; 
        return EVENT_TIERS.S_SILVER;
    }

    // A-Tier: Major Championships
    // Check level explicit properties
    if (level === 'state' || level === 'national' || level === 'region' || type === 'championship' || type === 'state championship') {
         return EVENT_TIERS.A_TIER;
    }
    
    // Fallback for names if level is generic 'Other' but name implies Champs
    if ((name.includes('championship') || name.includes('regional')) && !name.includes('qualifier') && !type.includes('qualifier')) {
         return EVENT_TIERS.A_TIER;
    }

    // C-Tier: Low Stakes (Check Type AND Name)
    if (
        type === 'scrimmage' || type === 'workshop' || type === 'practice' || type.includes('league session') ||
        name.includes('scrimmage') || name.includes('workshop') || name.includes('practice') || name.includes('league session')
    ) {
        return EVENT_TIERS.C_TIER;
    }
    
    // B-Tier: Standard Events
    // Tournaments, Qualifiers, League Finals
    return EVENT_TIERS.B_TIER;
}


/**
 * Rank tiers for Meow Score
 */
export const MEOW_RANKS = {
    MASTER: { min: 85, max: 100, name: 'Master', color: '#FF4757', emoji: '👑' },
    DIAMOND: { min: 75, max: 84, name: 'Diamond', color: '#5F27CD', emoji: '💎' },
    PLATINUM: { min: 65, max: 74, name: 'Platinum', color: '#00D2D3', emoji: '⭐' },
    GOLD: { min: 55, max: 64, name: 'Gold', color: '#FFC312', emoji: '🥇' },
    SILVER: { min: 45, max: 54, name: 'Silver', color: '#C4C4C4', emoji: '🥈' },
    BRONZE: { min: 35, max: 44, name: 'Bronze', color: '#CD7F32', emoji: '🥉' },
    IRON: { min: 0, max: 34, name: 'Iron', color: '#7F8C8D', emoji: '⚙️' }
};

/**
 * Get rank tier from Meow Score
 */
export function getMeowRank(score) {
    if (score >= MEOW_RANKS.MASTER.min) return MEOW_RANKS.MASTER;
    if (score >= MEOW_RANKS.DIAMOND.min) return MEOW_RANKS.DIAMOND;
    if (score >= MEOW_RANKS.PLATINUM.min) return MEOW_RANKS.PLATINUM;
    if (score >= MEOW_RANKS.GOLD.min) return MEOW_RANKS.GOLD;
    if (score >= MEOW_RANKS.SILVER.min) return MEOW_RANKS.SILVER;
    if (score >= MEOW_RANKS.BRONZE.min) return MEOW_RANKS.BRONZE;
    return MEOW_RANKS.IRON;
}

/**
 * Calculate Meow Score for a team
 * 
 * @param {Object} stats - Team statistics
 * @returns {number} Meow Score (0-100)
 */
export function calculateMeowScore(stats) {
    const {
        wins = 0,
        losses = 0,
        ties = 0,
        opr = 0,
        dpr = 0,
        ccwm = 0,
        ap = 0,
        wp = 0,
        avgOpponentRating = 50,
        isVexIQ = false
    } = stats;

    const matches = wins + losses + ties;

    // If no matches, return low baseline
    if (matches === 0) {
        return 30;
    }

    // Program-specific max values - adjusted for realistic distributions
    // These represent "excellent" performance, not theoretical maximums
    const maxOPR = isVexIQ ? 120 : 90;
    const maxDPR = isVexIQ ? 60 : 45;
    const maxCCWM = isVexIQ ? 60 : 45;


    // Base Score Components (Total Max 70)
    // 1. Win Performance Score (16 points)
    // Win Rate * 16. Bonus for ties.
    const winPerformance = Math.min(16, 16 * (wins / matches) * Math.min(1, matches / 10) + 0.5 * ties);

    // 2. Offensive Power Score (13 points)
    // Proportional to maxOPR
    const offensiveScore = Math.min(13, (opr / maxOPR) * 13);

    // 3. Defensive Power Score (13 points)
    // Lower is better. 0 DPR = 13 pts. Max DPR = 0 pts.
    const defensiveScore = Math.min(13, Math.max(0, 13 * (maxDPR - dpr) / maxDPR));

    // 4. Win Margin Score (10 points)
    const normalizedCCWM = Math.max(0, ccwm / maxCCWM);
    const winMarginScore = Math.min(10, normalizedCCWM * 10);

    // 5. Ranking Points Score (10 points) - Reduced from 12
    // Standard AP depends on matches played (6 AP per match standard - Auto Bonus)
    const standardAP = Math.max(1, matches * 6);
    const apScore = Math.min(6, (ap / standardAP) * 6);
    
    // Standard WP depends on matches played (2 WP per match standard)
    const standardWP = Math.max(1, matches * 2);
    const wpScore = Math.min(4, (wp / standardWP) * 4);
    
    const rankingPointsScore = apScore + wpScore;

    // 6. Strength of Schedule (REMOVED)

    // Bonus Components (Total Max 30)
    // 7. AWP Rate Score (15 points)
    // AWP is a bonus win point (1 per match). Rate = awp / matches.
    const awpRate = matches > 0 ? (stats.awp || 0) / matches : 0;
    const awpScore = Math.min(15, awpRate * 15);

    // 8. Programming Skills Score (15 points)
    // Using simple linear scaling relative to max expected
    const maxProg = isVexIQ ? 60 : 40; 
    const progScore = Math.min(15, ((stats.programming || 0) / maxProg) * 15);

    // Calculate raw total (Max ~92 now without SoS)
    let rawScore =
        winPerformance +
        offensiveScore +
        defensiveScore +
        winMarginScore +
        rankingPointsScore +
        awpScore +
        progScore;

    // Excellence Bonus (3 points) - Reduced
    if (
        matches >= 8 &&
        (wins / matches) > 0.75 &&
        opr > maxOPR * 0.7 &&
        dpr < maxDPR * 0.3 &&
        ccwm > maxCCWM * 0.7
    ) {
        rawScore += 3;
    }

    // New teams penalty (< 3 matches)
    if (matches < 3) {
        rawScore = rawScore * 0.7;
    }

    // Apply distribution curve
    // Adjusted to be less aggressive at the top end
    const applyCurve = (score) => {
        // Normalize to 0-1
        const normalized = Math.max(0, Math.min(100, score)) / 100;

        // Less aggressive sigmoid, shifted center to 40% to boost scores
        const x = (normalized - 0.4) * 12; // Reduced slope from 20, center from 0.5
        const sigmoid = 1 / (1 + Math.exp(-x)); 

        return sigmoid * 100;
    };

    let finalScore = Math.round(applyCurve(rawScore));

    // Ensure bounds
    return Math.max(0, Math.min(100, finalScore));
}

/**
 * Calculate average OPR, DPR, CCWM across multiple events
 * 
 * @param {Array} eventStats - Array of event-level stats objects with opr, dpr, ccwm
 * @returns {Object} Average stats
 */
export function calculateAverageStats(eventStats) {
    if (!eventStats || eventStats.length === 0) {
        return { opr: 0, dpr: 0, ccwm: 0 };
    }

    const sum = eventStats.reduce((acc, stats) => ({
        opr: acc.opr + (stats.opr || 0),
        dpr: acc.dpr + (stats.dpr || 0),
        ccwm: acc.ccwm + (stats.ccwm || 0)
    }), { opr: 0, dpr: 0, ccwm: 0 });

    return {
        opr: sum.opr / eventStats.length,
        dpr: sum.dpr / eventStats.length,
        ccwm: sum.ccwm / eventStats.length
    };
}

/**
 * Calculate Event Score Contribution (Meow Score Gain/Loss)
 * 
 * Algorithm:
 * 1. Calculate Base Meow Score from stats (0-100)
 * 2. Apply Tier Offset to Base Score (Difficulty Adjustment)
 * 3. Center score around 50 (Effective Base - 50 = Delta)
 * 4. Apply Swing Factor based on performance direction:
 *    - If Delta > 0 (Good): Multiplier = 1 + Swing
 *    - If Delta <= 0 (Bad): Multiplier = 1 - Swing
 * 5. Final Result = Delta * Multiplier
 * 
 * @param {Object} stats - Team statistics for the event
 * @param {Object} event - Event object
 * @returns {Object} Result object { scoreDelta, baseScore, tier }
 */
export function calculateEventScore(stats, event) {
    const baseScore = calculateMeowScore(stats);
    const tier = getEventTier(event);
    
    // Apply Tier Offset (Difficulty Adjustment)
    const effectiveBase = baseScore + (tier.offset || 0);

    // Center around 50
    const rawDelta = effectiveBase - 50;
    
    let multiplier = 1;
    if (rawDelta > 0) {
        // Did good
        multiplier = 1 + tier.swing; 
    } else {
        // Did bad (or average)
        multiplier = 1 - tier.swing;
    }

    const scoreDelta = rawDelta * multiplier;

    return {
        scoreDelta: parseFloat(scoreDelta.toFixed(2)),
        baseScore,
        tier
    };
}
