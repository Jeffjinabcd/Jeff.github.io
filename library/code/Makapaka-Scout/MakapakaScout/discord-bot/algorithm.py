ALGORITHM_TEXT = """
**MeowScore Algorithm: Mathematical Definition**

**1. Constants**
*VRC (High Stakes)*
Standard_OPR = 90
Standard_DPR = 45
Standard_CCWM = 45
Standard_WP = 2 per match
Standard_AP = 6 per match
Standard_Prog = 40

*VEX IQ (VIQ)*
Standard_OPR = 120
Standard_DPR = 60
Standard_CCWM = 60
Standard_WP = 2 per match
Standard_AP = 6 per match
Standard_Prog = 60

**2. Base Score Calculation**
N = Total Matches (Wins + Losses + Ties)

*Win Performance (Max 16)*
Win_Score = min(16, 16 * (Wins / N) * min(1, N / 10) + 0.5 * Ties)

*Offensive Score (Max 13)*
OPR_Score = min(13, (OPR / Standard_OPR) * 13)

*Defensive Score (Max 13) - Lower is better*
DPR_Score = min(13, max(0, 13 * (Standard_DPR - DPR) / Standard_DPR))

*Win Margin Score (Max 10)*
CCWM_Score = min(10, max(0, CCWM / Standard_CCWM) * 10)

*Ranking Score (Max 10)*
Standard_WP = max(1, N * 2)
Standard_AP = max(1, N * 6)
Rank_Score = min(6, (AP / Standard_AP) * 6) + min(4, (WP / Standard_WP) * 4)

*AWP Score (Max 15)*
AWP_Score = min(15, (AWP_Count / N) * 15)

*Programming Score (Max 15)*
Prog_Score = min(15, (Prog_Skills_Score / Standard_Prog) * 15)

**Raw Summation**
Raw_Score = Win_Score + OPR_Score + DPR_Score + CCWM_Score + Rank_Score + AWP_Score + Prog_Score

**3. Modifiers**

*Excellence Bonus (+3)*
Matches >= 8, Win Rate > 75%, Strong OPR/DPR/CCWM stats.

*Sample Size Penalty*
If Matches < 3: Raw_Score = Raw_Score * 0.7

*Distribution Curve (Sigmoid)*
x = (Raw_Score / 100 - 0.4) * 12
Final_Score = (1 / (1 + e^-x)) * 100

**4. Event Tier Adjustment**
Base_Score = Calculated MeowScore
Effective = Base_Score + Offset
Delta = Effective - 50

If Delta > 0: Multiplier = 1 + Tier_Swing
If Delta <= 0: Multiplier = 1 - Swing

Event_Score = 50 + (Delta * Multiplier)

*Tiers*
S-Gold (High Stakes Signatures): Offset = +15, Swing = +0.5
S-Silver (Mid Signatures): Offset = +5, Swing = +0.25
S-Bronze: Offset = 0, Swing = +0.1
A-Tier (Champs): Offset = 0, Swing = 0.0
B-Tier (Local Qualifiers): Offset = -10, Swing = -0.5
C-Tier (Scrims): Offset = -30, Swing = -0.8
"""
