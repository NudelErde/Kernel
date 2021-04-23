#pragma once

inline unsigned long long int operator ""Ki(unsigned long long int t) {
    return t * 1024;
}

inline unsigned long long int operator ""Mi(unsigned long long int t) {
    return t * 1024Ki;
}

inline unsigned long long int operator ""Gi(unsigned long long int t) {
    return t * 1024Mi;
}