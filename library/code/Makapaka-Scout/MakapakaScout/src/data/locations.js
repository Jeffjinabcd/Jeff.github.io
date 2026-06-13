export const COUNTRY_CODES = {
    "United States": "US", "China": "CN", "Canada": "CA", "Australia": "AU", "United Kingdom": "GB",
    "Taiwan": "TW", "Singapore": "SG", "Korea, Republic of": "KR", "New Zealand": "NZ", "Colombia": "CO",
    "Mexico": "MX", "Puerto Rico": "PR", "Philippines": "PH", "Thailand": "TH", "Japan": "JP",
    "Malaysia": "MY", "Spain": "ES", "Germany": "DE", "Turkey": "TR", "Paraguay": "PY",
    "Kazakhstan": "KZ", "Vietnam": "VN", "Hong Kong": "HK", "Macau": "MO"
};

export const COUNTRIES = Object.keys(COUNTRY_CODES).sort();

export const REGIONS = {
    "United States": [
        "Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "Florida", "Georgia", 
        "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", 
        "Massachusetts", "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", 
        "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", 
        "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington", 
        "West Virginia", "Wisconsin", "Wyoming", "District of Columbia", "Puerto Rico"
    ].sort(),
    "Canada": ["Alberta", "British Columbia", "Manitoba", "New Brunswick", "Newfoundland and Labrador", "Nova Scotia", "Ontario", "Prince Edward Island", "Quebec", "Saskatchewan"].sort(),
    "China": ["Beijing", "Shanghai", "Guangdong", "Jiangsu", "Zhejiang", "Shandong", "Sichuan", "Fujian", "Shaanxi", "Hunan", "Hubei", "Henan", "Hebei", "Liaoning", "Anhui", "Tianjin", "Chongqing", "Macau", "Hong Kong"].sort(),
    "Australia": ["New South Wales", "Victoria", "Queensland", "South Australia", "Western Australia", "Tasmania", "Australian Capital Territory"].sort(),
    "United Kingdom": ["England", "Scotland", "Wales", "Northern Ireland"].sort()
};
