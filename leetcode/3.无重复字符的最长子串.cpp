/*
 * @lc app=leetcode.cn id=3 lang=cpp
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int maxLen=0;
        for(int i=0;i<s.size();i++)
        {
            string sum;
            for(int j=i;j<s.size();j++)
            {
                if(sum.find(s[j])==std::string::npos)
                {
                sum.push_back(s[j]);
                if(sum.size()>maxLen)
                maxLen=sum.size();
                }
                else{
                    break;
                }
            }
        }
        return maxLen;
    }
};
// @lc code=end

