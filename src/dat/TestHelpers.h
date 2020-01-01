#pragma once

#define COMPARE_MEMBER(MEMBER) \
    if (this->MEMBER != other.MEMBER) { \
        std::cout << #MEMBER " differs in " << __FILE__  << ":" << __LINE__ << std::endl; \
        return false; \
    }


#define COMPARE_MEMBER_VEC(MEMBER) \
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
