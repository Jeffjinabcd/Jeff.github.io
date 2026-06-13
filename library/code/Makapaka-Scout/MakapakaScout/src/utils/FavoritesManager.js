import AsyncStorage from '@react-native-async-storage/async-storage';

const FAVORITES_KEY = '@vexhub_favorites';

export const FavoritesManager = {
    // Get all favorites
    getFavorites: async () => {
        try {
            const jsonValue = await AsyncStorage.getItem(FAVORITES_KEY);
            return jsonValue != null ? JSON.parse(jsonValue) : { teams: [], events: [] };
        } catch (e) {
            console.error("Failed to fetch favorites", e);
            return { teams: [], events: [] };
        }
    },

    // Add a favorite (team or event)
    addFavorite: async (item, type) => {
        try {
            const favorites = await FavoritesManager.getFavorites();

            if (type === 'team') {
                // Check if already exists
                if (!favorites.teams.some(t => t.id === item.id)) {
                    favorites.teams.push(item);
                }
            } else if (type === 'event') {
                if (!favorites.events.some(e => e.id === item.id)) {
                    favorites.events.push(item);
                }
            }

            await AsyncStorage.setItem(FAVORITES_KEY, JSON.stringify(favorites));
            return favorites;
        } catch (e) {
            console.error("Failed to add favorite", e);
            return null;
        }
    },

    // Remove a favorite
    removeFavorite: async (id, type) => {
        try {
            const favorites = await FavoritesManager.getFavorites();

            if (type === 'team') {
                favorites.teams = favorites.teams.filter(t => t.id !== id);
            } else if (type === 'event') {
                favorites.events = favorites.events.filter(e => e.id !== id);
            }

            await AsyncStorage.setItem(FAVORITES_KEY, JSON.stringify(favorites));
            return favorites;
        } catch (e) {
            console.error("Failed to remove favorite", e);
            return null;
        }
    },

    // Check if an item is favorited
    isFavorite: async (id, type) => {
        try {
            const favorites = await FavoritesManager.getFavorites();
            if (type === 'team') {
                return favorites.teams.some(t => t.id === id);
            } else if (type === 'event') {
                return favorites.events.some(e => e.id === id);
            }
            return false;
        } catch (e) {
            return false;
        }
    }
};
