#pragma once

#include <iostream>

#define COMPARE_MEMBER(MEMBER) \
    if (this->MEMBER != other.MEMBER) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (" << this->MEMBER << " vs. other " << other.MEMBER << ")" << std::endl; \
        return false; \
    }

#define COMPARE_MEMBER_ENUM_CLASS(MEMBER) \
    if (this->MEMBER != other.MEMBER) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (" << int(this->MEMBER) << " vs. other " << int(other.MEMBER) << ")" << std::endl; \
        return false; \
    }

#define COMPARE_MEMBER_PAIR(MEMBER) \
    if (this->MEMBER.first != other.MEMBER.first) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this first " << this->MEMBER.first << " vs. other " << other.MEMBER.first << ")" << std::endl; \
        return false; \
    } \
    if (this->MEMBER.second != other.MEMBER.second) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this second " << this->MEMBER.second << " vs. other " << other.MEMBER.second << ")" << std::endl; \
        return false; \
    } \

#define COMPARE_MEMBER_XYF(MEMBER) \
    if (this->MEMBER.x != other.MEMBER.x) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this x " << this->MEMBER.x << " vs. other " << other.MEMBER.x << ")" << std::endl; \
        return false; \
    } \
    if (this->MEMBER.y != other.MEMBER.y) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this y " << this->MEMBER.y << " vs. other " << other.MEMBER.y << ")" << std::endl; \
        return false; \
    } \

#define COMPARE_MEMBER_XYZF(MEMBER) \
    if (this->MEMBER.x != other.MEMBER.x) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this x " << this->MEMBER.x << " vs. other " << other.MEMBER.x << ")" << std::endl; \
        return false; \
    } \
    if (this->MEMBER.y != other.MEMBER.y) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this y " << this->MEMBER.y << " vs. other " << other.MEMBER.y << ")" << std::endl; \
        return false; \
    } \
    if (this->MEMBER.z != other.MEMBER.z) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << " (this z " << this->MEMBER.z << " vs. other " << other.MEMBER.z << ")" << std::endl; \
        return false; \
    } \

#define COMPARE_MEMBER_VEC(MEMBER) \
    if (MEMBER.size() != other.MEMBER.size()) { \
        std::cout << #MEMBER << " size differs, this " << MEMBER.size() << " vs " << other.MEMBER.size() << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        return false; \
    } \
    for (size_t i=0; i<MEMBER.size(); i++) { \
        if (MEMBER[i] != other.MEMBER[i]) { \
            std::cout << #MEMBER " vector differs in " << __FILE__  << ":" << __LINE__ << std::endl; \
            return false; \
        } \
    }

#define COMPARE_MEMBER_OBJ_VEC(MEMBER) \
    if (MEMBER.size() != other.MEMBER.size()) { \
        std::cout << #MEMBER << " size differs, this " << MEMBER.size() << " vs " << other.MEMBER.size() << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        return false; \
    } \
    for (size_t i=0; i<MEMBER.size(); i++) { \
        if (!MEMBER[i].compareTo(other.MEMBER[i])) { \
            std::cout << #MEMBER " vector differs in " << __FILE__  << ":" << __LINE__ << std::endl; \
            return false; \
        } \
    }

#define COMPARE_MEMBER_VEC_VEC(MEMBER) \
    if (MEMBER.size() != other.MEMBER.size()) { \
        std::cout << "this " << MEMBER.size() << " size diff vs " << other.MEMBER.size() << " at " << __FILE__  << ":" << __LINE__<< std::endl; \
        return false; \
    } \
    for (size_t i=0; i<MEMBER.size(); i++) { \
        if (MEMBER[i].size() != other.MEMBER[i].size()) { \
            std::cout << #MEMBER " vector differs in " << __FILE__  << ":" << __LINE__<< std::endl; \
            return false; \
        } \
        for (size_t j=0; j<MEMBER[i].size(); j++) { \
            if (!MEMBER[i][j].compareTo(other.MEMBER[i][j])) { \
                std::cout << #MEMBER " vector differs in " << __FILE__  << ":" << __LINE__<< std::endl; \
                return false; \
            } \
        } \
    }
