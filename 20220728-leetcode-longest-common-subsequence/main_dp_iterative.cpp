// https://leetcode.com/problems/longest-common-subsequence/

#include <array>
#include <iostream>
#include <string>


class Solution {
    public:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        std::array<std::array<int, 1001>, 1001> cache = {};
        for (int start1 = text1.size() - 1; start1 >= 0; start1--) {
            for (int start2 = text2.size() - 1; start2 >= 0; start2--) {
                if (text1[start1] == text2[start2]) {
                    // Case character is same at text1[start1] and text2[start2], then we take it. There's no reason for us to skip
                    cache[start1][start2] = 1 + cache[start1 + 1][start2 + 1];
        } else {
                    // Case character is different. We either skip a character from text1, or skip a character from text2.
                    cache[start1][start2] = std::max(cache[start1 + 1][start2], cache[start1][start2 + 1]);
                }
            }
        }
        return cache[0][0];
    }
};


int main() {
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("abcde", "ace") << std::endl;                        // ans: 3 ("ace")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("abc", "def") << std::endl;                          // ans: 0 ("")
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
