
const BASE_LAYOUT = {
    spacing: {
        xs: 4,
        s: 8,
        m: 16,
        l: 24,
        xl: 32
    },
    borderRadius: {
        s: 4,
        m: 8,
        l: 12,
        xl: 20
    },
    typography: {
        fontFamily: 'Rajdhani_600SemiBold',
        headerWeight: '700',
        bodyWeight: '500',
        sizes: {
            xs: 10,
            s: 12,
            m: 14,
            l: 16,
            xl: 20,
            xxl: 24,
            huge: 32
        }
    }
};

export const THEME_IDS = {
    DEFAULT: 'default',
    CRIMSON: 'crimson',
    OCEAN: 'ocean',
    FOREST: 'forest',
    AMETHYST: 'amethyst',
    SUNSET: 'sunset'
};

const createTheme = (id, name, primaryColor, cardBorderColor, extraColors = {}) => ({
    ...BASE_LAYOUT,
    id,
    name,
    colors: {
        background: '#111112', 
        surface: '#1A1A1C', 
        primary: primaryColor,
        secondary: '#FFFFFF',
        text: {
            primary: '#FFFFFF',
            secondary: '#A1A1AA',
            inverse: '#000000'
        },
        border: '#27272A',
        success: primaryColor,
        error: '#EF4444',
        blueAlliance: '#60A5FA',
        cardBorder: cardBorderColor,
        ...extraColors
    }
});

export const THEMES = {
    [THEME_IDS.DEFAULT]: createTheme(THEME_IDS.DEFAULT, 'Makapaka', '#D2FF00', 'rgba(210, 255, 0, 0.1)'),
    [THEME_IDS.CRIMSON]: createTheme(THEME_IDS.CRIMSON, 'Red', '#EF4444', 'rgba(239, 68, 68, 0.1)', { error: '#FF0000', success: '#EF4444' }),
    [THEME_IDS.OCEAN]: createTheme(THEME_IDS.OCEAN, 'Blue', '#3B82F6', 'rgba(59, 130, 246, 0.1)', { blueAlliance: '#2563EB' }),
    [THEME_IDS.FOREST]: createTheme(THEME_IDS.FOREST, 'Green', '#10B981', 'rgba(16, 185, 129, 0.1)'),
    [THEME_IDS.AMETHYST]: createTheme(THEME_IDS.AMETHYST, 'Purple', '#A855F7', 'rgba(168, 85, 247, 0.1)'),
    [THEME_IDS.SUNSET]: createTheme(THEME_IDS.SUNSET, 'Orange', '#F97316', 'rgba(249, 115, 22, 0.1)'),
};

// Legacy export for backward compatibility during refactor
export const THEME = THEMES[THEME_IDS.DEFAULT];

