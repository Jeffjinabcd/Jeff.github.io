
import React, { createContext, useContext, useState, useEffect } from 'react';
import AsyncStorage from '@react-native-async-storage/async-storage';
import { THEMES, THEME_IDS } from '../theme';

const ThemeContext = createContext({
    theme: THEMES[THEME_IDS.DEFAULT],
    themeName: THEME_IDS.DEFAULT,
    setTheme: () => {}
});

export const ThemeProvider = ({ children }) => {
    const [themeName, setThemeName] = useState(THEME_IDS.DEFAULT);

    useEffect(() => {
        // Load saved theme
        const loadTheme = async () => {
            try {
                const savedTheme = await AsyncStorage.getItem('user_theme');
                if (savedTheme && THEMES[savedTheme]) {
                    setThemeName(savedTheme);
                }
            } catch (e) {
                console.error('Failed to load theme', e);
            }
        };
        loadTheme();
    }, []);

    const setTheme = async (name) => {
        if (THEMES[name]) {
            setThemeName(name);
            try {
                await AsyncStorage.setItem('user_theme', name);
            } catch (e) {
                console.error('Failed to save theme', e);
            }
        }
    };

    const theme = THEMES[themeName];

    return (
        <ThemeContext.Provider value={{ theme, themeName, setTheme }}>
            {children}
        </ThemeContext.Provider>
    );
};

export const useTheme = () => useContext(ThemeContext);
