// https://leetcode.com/problems/longest-common-subsequence/

#include <iostream>
#include <string>
#include <vector>


class Solution {
    private:
    const int NA = -1;
    std::vector<std::vector<int>> cache = std::vector<std::vector<int>>(1000, std::vector<int>(1000, NA));

    int find_lcs(const std::string& text1, const std::string& text2, int start1, int start2) {
        if (start1 >= text1.size() || start2 >= text2.size()) return 0;
        if (cache[start1][start2] != NA) return cache[start1][start2];
        int lcs;
        if (text1[start1] == text2[start2]) {
            // Case character is same at text1[start1] and text2[start2], then we take it. There's no reason for us to skip.
            lcs = 1 + find_lcs(text1, text2, start1 + 1, start2 + 1);
        } else {
            // Case character is different. We either skip a character from text1, or skip a character from text2.
            lcs = std::max(find_lcs(text1, text2, start1 + 1, start2), find_lcs(text1, text2, start1, start2 + 1));
        }
        cache[start1][start2] = lcs;
        return lcs;
    }

    public:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        return find_lcs(text1, text2, 0, 0);
    }
};


int main() {
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("abcde", "ace") << std::endl;                        // ans: 3 ("ace")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("ezupkr", "ubmrapg") << std::endl;                   // ans: 2 ("ur")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("hofubmnylkra", "pqhgxgdofcvmr") << std::endl;       // ans: 5 ("hofmr")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("fcvafurqjylclorwfoladwfqzkbebslwnmpmlkbezkxoncvwhstwzwpqxqtyxozkpgtgtsjobujezgrkvevklmludgtyrmjaxyputqbyxqvupojutsjwlwluzsbmvyxifqtglwvcnkfsfglwjwrmtyxmdgjifyjwrsnenuvsdedsbqdovwzsdghclcdexmtsbexwrszihcpibwpidixmpmxshwzmjgtadmtkxqfkrsdqjcrmxkbkfoncrcvoxuvcdytajgfwrcxivixanuzerebuzklyhezevonqdsrkzetsrgfgxibqpmfuxcrinetyzkvudghgrytsvwzkjulmhanankxqfihenuhmfsfkfepibkjmzybmlkzozmluvybyzsleludsxkpinizoraxonmhwtkfkhudizepyzijafqlepcbihofepmjqtgrsxorunshgpazovuhktatmlcfklafivivefyfubunszyvarcrkpsnglkduzaxqrerkvcnmrurkhkpargvcxefovwtapedaluhclmzynebczodwropwdenqxmrutuhehadyfspcpuxyzodifqdqzgbwhodcjonypyjwbwxepcpujerkrelunstebopkncdazexsbezmhynizsvarafwfmnclerafejgnizcbsrcvcnwrolofyzulcxaxqjqzunedidulspslebifinqrchyvapkzmzwbwjgbyrqhqpolwjijmzyduzerqnadapudmrazmzadstozytonuzarizszubkzkhenaxivytmjqjgvgzwpgxefatetoncjgjsdilmvgtgpgbibexwnexstipkjylalqnupexytkradwxmlmhsnmzuxcdkfkxyfgrmfqtajatgjctenqhkvyrgvapctqtyrufcdobibizihuhsrsterozotytubefutaxcjarknynetipehoduxyjstufwvkvwvwnuletybmrczgtmxctuny", "nohgdazargvalupetizezqpklktojqtqdivcpsfgjopaxwbkvujilqbclehulatshehmjqhyfkpcfwxovajkvankjkvevgdovazmbgtqfwvejczsnmbchkdibstklkxarwjqbqxwvixavkhylqvghqpifijohudenozotejoxavkfkzcdqnoxydynavwdylwhatslyrwlejwdwrmpevmtwpahatwlaxmjmdgrebmfyngdcbmbgjcvqpcbadujkxaxujudmbejcrevuvcdobolcbstifedcvmngnqhudixgzktcdqngxmruhcxqxypwhahobudelivgvynefkjqdyvalmvudcdivmhghqrelurodwdsvuzmjixgdexonwjczghalsjopixsrwjixuzmjgxydqnipelgrivkzkxgjchibgnqbknstspujwdydszohqjsfuzstyjgnwhsrebmlwzkzijgnmnczmrehspihspyfedabotwvwxwpspypctizyhcxypqzctwlspszonsrmnyvmhsvqtkbyhmhwjmvazaviruzqxmbczaxmtqjexmdudypovkjklynktahupanujylylgrajozobsbwpwtohkfsxeverqxylwdwtojoxydepybavwhgdehafurqtcxqhuhkdwxkdojipolctcvcrsvczcxedglgrejerqdgrsvsxgjodajatsnixutihwpivihadqdotsvyrkxehodybapwlsjexixgponcxifijchejoxgxebmbclczqvkfuzgxsbshqvgfcraxytaxeviryhexmvqjybizivyjanwxmpojgxgbyhcruvqpafwjslkbohqlknkdqjixsfsdurgbsvclmrcrcnulinqvcdqhcvwdaxgvafwravunurqvizqtozuxinytafopmhchmxsxgfanetmdcjalmrolejidylkjktunqhkxchyjmpkvsfgnybsjedmzkrkhwryzan") << std::endl;
                                                                                                            // ans: 323
    }
}
