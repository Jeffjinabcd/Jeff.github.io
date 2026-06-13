export const DEFAULT_SEASON_ID = 190;
export const PROGRAM_ID = 1; // VRC

export const CATEGORIES = [
    { id: 'v5hs', label: 'V5HS', programId: 1, grade: 'High School' },
    { id: 'v5ms', label: 'V5MS', programId: 1, grade: 'Middle School' },
    { id: 'vexiq', label: 'VEXIQ', programId: 41, grade: 'All' },
    { id: 'vexu', label: 'VEXU', programId: 4, grade: 'College' }
];

export const MANUAL_DATA = [
    {
        id: 'scoring',
        title: 'Scoring Rules',
        content: [
            { code: 'SC1', name: 'Scoring Evaluation', text: 'All Scoring statuses are evaluated after the Match ends. Scores are calculated five (5) seconds after the Match ends, or once all Blocks, Field Elements, and Robots on the Field come to rest, whichever comes first.' },
            { code: 'SC2', name: 'Scored Block', text: 'A Block is considered Scored if it meets all of the following criteria: a. The Block is in contact with the inside surface(s) of the clear plastic part of a Goal. b. The Block is not in contact with a Robot of the same color as that Block. c. The Block is not in contact with the Floor.' },
            { code: 'SC3', name: 'Controlled Zone', text: 'A Control Zone is considered Controlled by an Alliance if a majority of the Blocks Scored in that Control Zone are the same color as the Alliance. For Long Goals, a Scored Block is considered Scored in the Control Zone if it is entirely contained within that Control Zone.' },
            { code: 'SC4', name: 'Parked Robot', text: 'A Robot is considered Parked if it meets all of the following criteria: a. The Robot is not contacting the Floor outside of its Alliance-colored Park Zone. b. The Robot is not contacting any Field Elements other than the inside face of the Field Perimeter, the Floor inside of its Alliance-colored Park Zone, and/or the plastic extrusions and connectors that are part of the Park Zone. c. The Robot is at least partially within the vertical projection of its Alliance-colored Park Zone.' },
            { code: 'SC5', name: 'Autonomous Bonus', text: 'Scoring of the Autonomous Bonus is evaluated immediately after the Autonomous Period ends (i.e., once all Blocks, Field Elements, and Robots on the Field come to rest). Points for Parked Robots are not included in the calculation of an Alliance\'s score for the purposes of determining the Autonomous Bonus.' },
            { code: 'SC6', name: 'Autonomous Win Point', text: 'An Autonomous Win Point is awarded to any Alliance that ends the Autonomous Period with all of the following tasks completed, and that has committed no Violations during the Autonomous Period: 1. At least seven (7) Blocks of the Alliance\'s color are Scored. 2. At least three (3) different Goals include at least one (1) Scored Block of the Alliance\'s color. 3. At least three (3) Blocks of the Alliance\'s color have been removed from Loaders adjacent to the Alliance\'s Alliance Station. 4. Neither Robot is contacting the Park Zone barrier.' }
        ]
    },
    {
        id: 'safety',
        title: 'Safety Rules',
        content: [
            { code: 'S1', name: 'Be safe out there', text: 'If at any time the Robot operation or Team actions are deemed unsafe or have damaged a Field Element, Block, or the Field, the offending Team may receive a Disablement and/or Disqualification at the discretion of the Head Referee.' },
            { code: 'S2', name: 'Adult Supervision', text: 'Students must be accompanied by an Adult. No Student may attend a V5RC event without a responsible Adult supervising them. The Adult must obey all rules and be careful to not violate Student-centered policies, but must be present for the full duration of the event in the case of an emergency.' },
            { code: 'S3', name: 'Stay inside the Field', text: 'If a Robot is completely outside of the Field during a match, it will receive a Disablement for the remainder of the Match. The intent of this rule is not to penalize Robots for having mechanisms that inadvertently cross the Field Perimeter during normal game play.' },
            { code: 'S4', name: 'Wear safety glasses', text: 'All Drive Team Members must wear safety glasses or glasses with side shields while at the Field for Matches. While in the pit and queuing areas, it is highly recommended that all Team members wear safety glasses.' },
            { code: 'S5', name: 'Release Forms', text: 'Each Student Team member must have a completed participant release form on file for the event and season. A Student Team member cannot participate in an event without a completed release form on file.' }
        ]
    },
    {
        id: 'general',
        title: 'General Rules',
        content: [
            { code: 'G1', name: 'Treat everyone with respect', text: 'All Teams are expected to conduct themselves in a respectful and professional manner while competing in VEX V5 Robotics Competition events. If a Team or any of its members are disrespectful or uncivil to event staff, volunteers, or fellow competitors, they may receive a Disqualification from a current or upcoming Match.' },
            { code: 'G2', name: 'Student-centered', text: 'V5RC is a student-centered program. Adults should not make decisions about the Robot\'s build, design, or gameplay, and should not provide an unfair advantage by providing help that is beyond the Student\'s independent abilities.' },
            { code: 'G3', name: 'Use common sense', text: 'When reading and applying the various rules in this document, please remember that common sense always applies in the VEX V5 Robotics Competition.' },
            { code: 'G4', name: 'Skill Level', text: 'All work must represent the skill level of the Students on the Team. The Team\'s design, Robot, coding, strategy, and ongoing work must represent the skill level of the Students currently on the Team. Teams must avoid academic dishonesty and should not copy a Robot or mechanism that has been provided for them.' },
            { code: 'G5', name: 'One Team per Student', text: 'Each Student can only belong to one Team. Each Team must include Drive Team Members, Coder(s), Designer(s), and Builder(s). No Student may fulfill any of these roles for more than one VEX V5 Robotics Competition Team in a given competition season.' }
        ]
    },
    {
        id: 'game_general',
        title: 'General Game Rules',
        content: [
            { code: 'GG1', name: 'Drive Team', text: 'Only Drive Team Members, and only in the Alliance Station. During a Match, Robots may be operated only by the Drive Team Members and/or by software running on the Robot\'s control system. A Team may send up to (3) Drive Team Members to their Alliance Station for each Robot.' },
            { code: 'GG2', name: 'Robot Attendance', text: 'A Team\'s Robot should attend every Match. The Team\'s Robot must report to the Field for the Team\'s assigned Match, even if the Robot is not functional.' },
            { code: 'GG3', name: 'Ready to Play', text: 'Robots on the Field must be ready to play. When a Team puts their Robot on the Field, it must be prepared to play (e.g., batteries charged, sized within the starting size constraint, includes only the correct Alliance-color license plates, etc.).' },
            { code: 'GG4', name: 'Hands out of Field', text: 'Drive Team Members are prohibited from making intentional contact with any Blocks, Field Elements, or Robots during a Match, apart from the contact specified in GG4a or while introducing Match Loads.' },
            { code: 'GG5', name: 'Match Replays', text: 'Match replays are allowed, but rare. Match replays must be agreed upon by both the Event Partner and Head Referee, and will only be issued in the most extreme circumstances.' },
            { code: 'GG6', name: 'Disqualifications', text: 'When a Team receives a Disqualification in a Qualification Match, they receive a score of zero (0) for the Match, as well as zero (0) Win Points, Autonomous Win Points, Autonomous Points, and Strength of Schedule Points.' },
            { code: 'GG7', name: 'Time Outs', text: 'Each Elimination Alliance gets one three-minute Time Out, which they may request during the Elimination Bracket. The Time Out will be served at the time of the Alliance\'s next upcoming Match.' },
            { code: 'GG8', name: 'Keep Robot Together', text: 'Keep your Robots together. Robots may not intentionally detach parts during the Match or leave mechanisms on the Field.' },
            { code: 'GG9', name: 'Don\'t Clamp Field', text: 'Robots may not intentionally grasp, grapple, hook, attach to or otherwise Entangle with any Field Elements. Strategies with mechanisms that react against multiple sides of a Field Element in an effort to latch or clamp onto said Field Element are prohibited.' },
            { code: 'GG10', name: 'Red Places Last', text: 'The red Alliance places last. The red Alliance has the right to place its Robots on the Field last in Qualification Matches and Elimination Matches.' },
            { code: 'GG11', name: 'Controllers Connected', text: 'Controllers must stay connected to the Field. Prior to the beginning of each Match, Drive Team Members must plug their V5 Controller into the Field\'s control system.' },
            { code: 'GG12', name: 'Autonomous', text: 'Autonomous means "no humans." During the Autonomous Period, Drive Team Members are not permitted to interact with the Robots in any way, directly or indirectly.' },
            { code: 'GG13', name: 'Auto Rules', text: 'All rules still apply in the Autonomous Period. Teams are responsible for the actions of their Robots at all times, including during the Autonomous Period. Any Violations committed during the Autonomous Period will result in the Autonomous Bonus being automatically awarded to the opposing Alliance.' },
            { code: 'GG14', name: 'Don\'t Destroy', text: 'Don\'t destroy other Robots. But, be prepared to encounter defense. Strategies aimed solely at the destruction, damage, tipping over, or Entanglement of opposing Robots are not part of the ethos of the VEX V5 Robotics Competition and are not allowed.' },
            { code: 'GG15', name: 'Offensive Advantage', text: 'Offensive Robots get the "benefit of the doubt." In a case where a Head Referee is forced to make a judgment call regarding a destructive interaction between a defensive and offensive Robot... referees will decide in favor of the offensive Robot.' },
            { code: 'GG16', name: 'Forcing Penalties', text: 'You can\'t force an opponent into a penalty. Intentional strategies that cause an opponent to break a rule are not permitted, and will not result in a Violation for the opposing Alliance.' },
            { code: 'GG17', name: 'Holding', text: 'No Holding for more than a 3-count. A Robot may not Hold an opposing Robot for more than a 3-count during the Driver Controlled Period. A Holding count should begin immediately once the Head Referee observes a suspected Holding interaction.' },
            { code: 'GG18', name: 'Use Blocks', text: 'Use Blocks to play the game. Blocks may not be used to accomplish actions that would be otherwise illegal if they were attempted by Robot mechanisms (e.g. using a Block to intentionally tip an opponent).' }
        ]
    },
    {
        id: 'specific',
        title: 'Specific Game Rules',
        content: [
            { code: 'SG1', name: 'Starting Position', text: 'Prior to the start of each Match, the Robot must be placed such that it meets all of the following criteria: a. No larger than 18" x 18" x 18". b. Not contacting any Blocks other than a maximum of one (1) Preload. c. Not contacting any Goals or Loaders. d. Not contacting any other Robots. e. Completely stationary. f. Contacting the barrier that defines their Alliance\'s Park Zone.' },
            { code: 'SG2', name: 'Horizontal Expansion', text: 'Horizontal expansion is limited. Once the Match begins, Robots may expand horizontally beyond the 18" starting size limit within the following criteria: a. The Robot can never be larger than 22" wide or 22" long.' },
            { code: 'SG3', name: 'Vertical Expansion', text: 'Vertical expansion is limited. Once the Match begins, Robots may expand vertically beyond the 18" starting size limit within the following criteria: a. No part of the Robot may exceed an overall height of 22" at any point during the Match.' },
            { code: 'SG4', name: 'Keep Blocks in Field', text: 'Keep Blocks in the Field. Teams may not remove Blocks from the Field. A Block that leaves the Field during Match play, intentionally or unintentionally, will be given to a Drive Team Member from the same color Alliance as the Block and may be used as a Match Load.' },
            { code: 'SG5', name: 'Preloads', text: 'Each Robot gets one Block as a Preload. Prior to the start of each Match, each Preload must be placed such that it meets all of the following criteria: a. Contacting one Robot of the same Alliance color. b. Not contacting the same Robot as another Preload. c. Not contacting or within the volume of a Goal or Loader.' },
            { code: 'SG6', name: 'Possession', text: 'A Robot may carry, push, or plow an unlimited number of Blocks. However, horizontal and vertical expansion limits apply to all parts of the Robot for the entire duration of the Match.' },
            { code: 'SG7', name: 'Auto Line', text: 'Don\'t cross the Autonomous Line, and don\'t interfere with your opponents\' actions. During the Autonomous Period, Robots may not contact foam tiles, Blocks, or Field Elements which are on the opposing Alliance\'s side of the Autonomous Line.' },
            { code: 'SG8', name: 'Auto Line Risk', text: 'Engage with the Autonomous Line at your own risk. Any Robot who engages with Blocks that begin the Match on the Autonomous Line should be aware that opponent Robots may also choose to do the same.' },
            { code: 'SG9', name: 'Match Loads', text: 'Match Loads may be introduced during the Match under certain conditions. Drive Team Members may introduce Match Load Blocks by placing them one at a time into either of the Loaders adjacent to their Alliance Station.' },
            { code: 'SG10', name: 'Enclosed Goals', text: 'Don\'t reach inside enclosed sections of Goals. Robots are not permitted to directly contact Blocks that are fully within enclosed sections of Long Goals, but may add or remove Blocks from open sections of Goals at any time during a Match.' },
            { code: 'SG11', name: 'Park Protection', text: 'Park Zones are protected during the endgame. During the last 20 seconds of a Match, Robots may not directly or indirectly contact the other Alliance\'s Park Zone or opponent Robots that are partially or entirely within the vertical projection of that Park Zone.' }
        ]
    },
    {
        id: 'robot',
        title: 'Robot Rules',
        content: [
            { code: 'R1', name: 'One Robot', text: 'One Robot per Team. Each Team can only bring one (1) Robot to a given event. A given Robot may only be used by one (1) Team.' },
            { code: 'R2', name: 'Skill Level', text: 'Robots must represent the Team\'s skill level. The Robot must be designed, built, and programmed by members of the Team.' },
            { code: 'R3', name: 'Inspection', text: 'Robots must pass inspection. Every Robot will be required to pass a full inspection before being cleared to compete.' },
            { code: 'R4', name: 'Violations', text: 'There is a difference between accidentally and willfully violating a Robot rule. Any Violation of Robot rules, accidental or intentional, will result in a Team being unable to play until they pass inspection.' },
            { code: 'R5', name: 'Size', text: 'Robots must fit within an 18" x 18" x 18" volume.' },
            { code: 'R6', name: 'License Plates', text: 'Officially registered Team numbers must be displayed on Robot license plates. License plates must be placed in fixed locations on exactly two (2) horizontally opposing sides of the Robot.' },
            { code: 'R7', name: 'Release Blocks', text: 'Let go of Blocks after the Match. Robots must be designed to permit easy removal of Blocks from any mechanism without requiring the Robot to have power.' },
            { code: 'R8', name: 'One Brain', text: 'Robots have one Brain. Robots must ONLY use one (1) VEX V5 Robot Brain.' },
            { code: 'R9', name: 'Power Access', text: 'Keep the power button accessible. The on/off button on the V5 Robot Brain and/or the Battery Cable connection must be accessible without moving or lifting the Robot.' },
            { code: 'R10', name: 'Firmware', text: 'Teams must use VEXos version 1.1.5 or newer.' },
            { code: 'R11', name: 'Template', text: 'Use a "Competition Template" for programming. The Robot must be programmed to follow control directions provided by the Field Controllers.' },
            { code: 'R12', name: 'Motors', text: 'Motors are limited. Robots may use any combination of VEX V5 Smart Motors (11W) and Smart Motors (5.5W) such that the combined power does not exceed 88W.' },
            { code: 'R13', name: 'Batteries', text: 'Electrical power comes from VEX batteries only. Robots may use one (1) V5 Robot Battery to power the V5 Robot Brain.' },
            { code: 'R14', name: 'Radio', text: 'Robots use VEXnet. Robots must ONLY utilize the VEXnet system for all wireless Robot communication.' },
            { code: 'R15', name: 'Radio Space', text: 'Give the radio some space. The V5 Radio must be mounted such that no metal surrounds the radio symbol on the V5 Radio.' },
            { code: 'R16', name: 'Controllers', text: 'One or two Controllers per Robot. No more than two (2) VEX V5 Controllers may control a single Robot.' },
            { code: 'R17', name: 'V5 System', text: 'Robots are built from the VEX V5 system. Robots may be built ONLY using official VEX V5 components, unless otherwise specifically noted.' },
            { code: 'R19', name: 'Prohibited', text: 'Prohibited Items include those that could damage Field Elements, pose safety hazards, or are from other product lines (VEX IQ, VEX EXP) unless cross-listed.' },
            { code: 'R20', name: 'Non-VEX', text: 'Certain non-VEX components are allowed, such as color filters, non-aerosol grease, anti-static compound, hot glue for cables, unlimited rope/string (1/4" max), and tape for wire management.' },
            { code: 'R25', name: 'Custom Plastic', text: 'A limited amount of custom plastic is allowed. Robots may use custom-made pieces cut from certain types of non-shattering plastic (e.g. Polycarbonate), up to 0.070" thick. Max 12 pieces, each no larger than 4" x 8".' },
            { code: 'R26', name: 'Pneumatics', text: 'Pneumatics are limited. A Robot\'s pneumatic subsystem may use a maximum of two (2) legal VEX pneumatic air reservoirs, charged to a maximum of 100 psi.' }
        ]
    },
    {
        id: 'skills',
        title: 'Robot Skills',
        content: [
            { code: 'RSC1', name: 'Standard Rules', text: 'Standard rules apply in most cases. Removing Blocks from the Field in a Robot Skills Match is not a Violation. Blocks that leave the Field cannot be returned.' },
            { code: 'RSC2', name: 'Scoring', text: 'Scoring Robot Skills Matches. Teams will receive points for all Scored Blocks, regardless of color. A Control Zone is considered filled if it ends the Match containing its maximum number of Scored Blocks, and if all Blocks in that Control Zone are the same color. Points are also awarded for Cleared Park Zones, Cleared Loaders, and Parked Robots.' },
            { code: 'RSC3', name: 'Setup', text: 'Robot and Field setup for Skills Matches. The Robot must start the Match in a legal starting position for the red Alliance. One red Block must be used as a Preload. 36 Blocks begin in unscored positions and 24 Blocks begin in Loaders.' },
            { code: 'RSC4', name: 'Stop Time', text: 'Skills Stop Time. If a Team wishes to end their Robot Skills Match early, they may elect to record a Skills Stop Time. This is used as a tiebreaker for Robot Skills Challenge rankings.' }
        ]
    },
    {
        id: 'tournament',
        title: 'Tournament Rules',
        content: [
            { code: 'T1', name: 'Head Referee', text: 'Head Referees have ultimate and final authority on all gameplay and Robot ruling decisions during the competition.' },
            { code: 'T3', name: 'Appeals', text: 'Drive Team Members are permitted to immediately appeal a Head Referee\'s ruling. If Drive Team Members wish to dispute a score or ruling, they must stay in the Alliance Station until the Head Referee talks with them.' },
            { code: 'T5', name: 'Field Variance', text: 'Be prepared for minor Field variance. Field Element tolerances and Blocks may vary from specified locations/dimensions by up to +/- 1.0 inch.' },
            { code: 'T11', name: 'Matches', text: 'Each Team will have at least six Qualification Matches.' },
            { code: 'T12', name: 'Rankings', text: 'Qualification Matches contribute to a Team\'s ranking for Alliance Selection based on Win Points (WP), Autonomous Points (AP), and Strength of Schedule Points (SP).' },
            { code: 'T17', name: 'Eliminations', text: 'Elimination Matches follow the Elimination Bracket. A sixteen (16) Alliance bracket plays through numbered order.' }
        ]
    },
    {
        id: 'vexu',
        title: 'VEX U Rules',
        content: [
            { code: 'VUG1', name: 'Placement', text: 'Different Robot placement than rule GG10. The red Team has the right to place one Robot on the Field first, followed by both blue Robots, and ending with the 2nd red Robot.' },
            { code: 'VUG2', name: 'Expansion', text: 'Different expansion. The designated 24" Robot may expand horizontally and/or vertically up to a maximum size of 24" x 24" x 24" at any time during the Match.' },
            { code: 'VUG4', name: 'AWP', text: 'Different Autonomous Win Point criteria. Awarded for: 12+ Blocks Scored, 3+ Goals with at least 1 Block, 6 Blocks removed from Loaders, and at least one Robot Parked.' },
            { code: 'VUR1', name: 'Robots', text: 'Teams may use two (2) Robots in each Match. One Robot must be no larger than 24"x24"x24", and the other no larger than 15"x15"x15".' },
            { code: 'VUR3', name: 'Fabrication', text: 'Fabricated Parts may be made by applying manufacturing processes such as 3D printing, machining, and welding to legal Raw Stock.' }
        ]
    },
    {
        id: 'vexai',
        title: 'VEX AI Rules',
        content: [
            { code: 'VAIG2', name: 'No Humans', text: 'Autonomous means "no humans." Drive Team Members are not permitted to interact with their Robots in any way while they are operating autonomously (i.e., during the entirety of a VAIRC Match).' },
            { code: 'VAISC4', name: 'Isolation WP', text: 'Isolation Win Point criteria. Awarded for: 9+ Blocks Scored, 3+ Goals with at least 1 Block, and neither Robot contacting the Park Zone barrier.' },
            { code: 'VAIRS4', name: 'Skills Start', text: 'A 24" Robot in a VAIRC Robot Skills Match begins in the blue Park Zone. A 15" Robot begins in the red Park Zone.' }
        ]
    }
];

export const DEMO_TEAMS = [
    { id: 1, number: '254', team_name: 'Cheesy Poofs', organization: 'Bellarmine', location: { region: 'California' }, grade: 'High School' },
    { id: 2, number: '1114', team_name: 'Simbotics', organization: 'Governor Simcoe', location: { region: 'Ontario' }, grade: 'High School' },
    { id: 3, number: '9999Z', team_name: 'Push Bots', organization: 'VEX U', location: { region: 'Texas' }, grade: 'High School' },
];

export const DEMO_EVENTS = [
    {
        id: 101,
        sku: 'RE-VRC-25-1234',
        name: 'Demo: V5RC Push Back Championship',
        start: '2025-04-25',
        location: { venue: 'Kay Bailey Hutchison', city: 'Dallas', region: 'TX' },
        season: { name: 'Push Back' }
    },
];
