import React, { useState, useEffect, useRef } from 'react';
import { View, Text, Image, StyleSheet, Animated, Dimensions } from 'react-native';
import { useTheme } from '../context/ThemeContext';
import { THEME } from '../theme';

const { width } = Dimensions.get('window');

const CAT_IMAGES = [
    require('../../assets/cat1.jpg'),
    require('../../assets/cat2.jpg'),
    require('../../assets/cat3.jpg'),
    require('../../assets/cat4.jpg'),
    require('../../assets/cat5.jpg'),
    require('../../assets/cat6.jpg'),
];

export const LoadingScreen = ({ progress = 0, message = 'Loading...' }) => {
    const { theme } = useTheme();
    const styles = React.useMemo(() => getStyles(theme), [theme]);
    const [randomCat, setRandomCat] = useState(null);
    const fadeAnim = useRef(new Animated.Value(0)).current;
    const progressAnim = useRef(new Animated.Value(0)).current;

    useEffect(() => {
        // Pick a random cat on mount
        const randomIndex = Math.floor(Math.random() * CAT_IMAGES.length);
        setRandomCat(CAT_IMAGES[randomIndex]);

        // Fade in
        Animated.timing(fadeAnim, {
            toValue: 1,
            duration: 500,
            useNativeDriver: true,
        }).start();
    }, []);

    useEffect(() => {
        // Animate progress bar
        Animated.timing(progressAnim, {
            toValue: progress,
            duration: 300,
            useNativeDriver: false, // Width cannot use native driver
        }).start();
    }, [progress]);

    if (!randomCat) return null;

    const widthInterpolation = progressAnim.interpolate({
        inputRange: [0, 1],
        outputRange: ['0%', '100%'],
    });

    return (
        <Animated.View style={[styles.container, { opacity: fadeAnim }]}>
            <View style={styles.content}>
                <Text style={styles.appName}>Makapaka Scout</Text>

                <View style={styles.imageContainer}>
                    <Image source={randomCat} style={styles.catImage} resizeMode="cover" />
                </View>

                <View style={styles.progressContainer}>
                    <Animated.View style={[styles.progressBar, { width: widthInterpolation }]} />
                </View>

                <Text style={styles.message}>{message} {Math.round(progress * 100)}%</Text>
            </View>
        </Animated.View>
    );
};

const getStyles = (THEME) => StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: THEME.colors.background,
        justifyContent: 'center',
        alignItems: 'center',
        position: 'absolute',
        top: 0,
        left: 0,
        right: 0,
        bottom: 0,
        zIndex: 9999,
    },
    content: {
        alignItems: 'center',
        width: '80%',
    },
    appName: {
        fontSize: 32,
        fontWeight: '900',
        color: THEME.colors.primary,
        marginBottom: 40,
        letterSpacing: 1,
    },
    imageContainer: {
        width: 200,
        height: 200,
        borderRadius: 100,
        overflow: 'hidden',
        borderWidth: 4,
        borderColor: THEME.colors.primary,
        marginBottom: 40,
        elevation: 10,
        shadowColor: THEME.colors.primary,
        shadowOffset: { width: 0, height: 4 },
        shadowOpacity: 0.3,
        shadowRadius: 8,
    },
    catImage: {
        width: '100%',
        height: '100%',
    },
    progressContainer: {
        width: '100%',
        height: 8,
        backgroundColor: THEME.colors.surface,
        borderRadius: 4,
        overflow: 'hidden',
        marginBottom: 16,
    },
    progressBar: {
        height: '100%',
        backgroundColor: THEME.colors.primary,
        borderRadius: 4,
    },
    message: {
        fontSize: 16,
        color: THEME.colors.text.secondary,
        fontWeight: '500',
    },
});
