// Helper to calculate win probability based on score difference
// Assuming normal distribution with std dev of ~20 points
export const calculateWinProbability = (scoreDiff) => {
    const stdDev = 20;
    const z = scoreDiff / (stdDev * Math.sqrt(2)); // Diff of two variables
    // Simple specific approximation for normal CDF
    const t = 1 / (1 + 0.3275911 * Math.abs(z));
    const factor = 0.254829592 * t + -0.284496736 * Math.pow(t, 2) + 1.421413741 * Math.pow(t, 3) + -1.453152027 * Math.pow(t, 4) + 1.061405429 * Math.pow(t, 5);
    const errorFn = 1 - factor * Math.exp(-z * z);
    const cdf = 0.5 * (1 + (z > 0 ? 1 : -1) * errorFn);

    return Math.round(cdf * 100);
};
