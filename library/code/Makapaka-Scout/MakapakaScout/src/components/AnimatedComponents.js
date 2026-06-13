import React, { useEffect, useRef } from 'react';
import { Animated, View } from 'react-native';

/**
 * Animated Tab Content Component
 * Animates tab content in/out with slide and fade transitions
 */
export const AnimatedTabContent = ({ children, isActive, index = 0, direction = 'horizontal' }) => {
    const fadeAnim = useRef(new Animated.Value(isActive ? 1 : 0)).current;
    const slideAnim = useRef(new Animated.Value(isActive ? 0 : (direction === 'horizontal' ? 100 : 50))).current;

    useEffect(() => {
        Animated.parallel([
            Animated.timing(fadeAnim, {
                toValue: isActive ? 1 : 0,
                duration: 250,
                useNativeDriver: true,
            }),
            Animated.timing(slideAnim, {
                toValue: isActive ? 0 : (direction === 'horizontal' ? (index % 2 === 0 ? -100 : 100) : 50),
                duration: 250,
                useNativeDriver: true,
            }),
        ]).start();
    }, [isActive]);

    if (!isActive && fadeAnim._value === 0) {
        return null;
    }

    const transformStyle = direction === 'horizontal'
        ? { transform: [{ translateX: slideAnim }] }
        : { transform: [{ translateY: slideAnim }] };

    return (
        <Animated.View
            style={[
                {
                    flex: 1,
                    opacity: fadeAnim,
                    position: 'absolute',
                    top: 0,
                    left: 0,
                    right: 0,
                    bottom: 0,
                },
                transformStyle,
                !isActive && { pointerEvents: 'none' }
            ]}
        >
            {children}
        </Animated.View>
    );
};

/**
 * Animated View Stack Component
 * Animates view transitions when navigating forward/back
 */
export const AnimatedViewTransition = ({ children, direction = 'forward' }) => {
    const slideAnim = useRef(new Animated.Value(direction === 'forward' ? 100 : -100)).current;
    const fadeAnim = useRef(new Animated.Value(0)).current;

    useEffect(() => {
        Animated.parallel([
            Animated.timing(slideAnim, {
                toValue: 0,
                duration: 300,
                useNativeDriver: true,
            }),
            Animated.timing(fadeAnim, {
                toValue: 1,
                duration: 250,
                useNativeDriver: true,
            }),
        ]).start();
    }, []);

    return (
        <Animated.View
            style={{
                flex: 1,
                opacity: fadeAnim,
                transform: [{ translateX: slideAnim }],
            }}
        >
            {children}
        </Animated.View>
    );
};
