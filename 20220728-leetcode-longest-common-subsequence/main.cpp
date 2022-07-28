// https://leetcode.com/problems/longest-common-subsequence/

#include <deque>
#include <iostream>
#include <limits>
#include <unordered_set>
#include <string>


class Solution {
private:
    struct QUEUE_T {
        int pos1, pos2;
        int cur_score;
    };
    struct HISTORY_T {
        int pos1, pos2;
        bool operator==(const HISTORY_T& other) const {
            return std::tie(pos1, pos2) == std::tie(other.pos1, other.pos2);
        }
        size_t operator()(const HISTORY_T& c) const {
            // return (std::hash<int>()(c.pos1)) ^ (std::hash<int>()(c.pos2));
            return ((uint64_t)c.pos1)<<32 | (uint64_t)c.pos2;
        }
    };
    std::unordered_set<HISTORY_T, HISTORY_T> history;


public:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        int answer = std::numeric_limits<int>::min();
        std::deque<QUEUE_T> queues;
        queues.push_back({0, 0, 0});
        while (! queues.empty()) {
            QUEUE_T queue = queues.front(); queues.pop_front();
            // We have reached end of string
            if (queue.pos1 == text1.length() || queue.pos2 == text2.length()) {
                answer = std::max(answer, queue.cur_score);
                continue;
            }
            // Prune : Do not process the same strings again
            bool is_in = history.find({queue.pos1, queue.pos2}) != history.end();
            if (is_in) continue;
            history.insert({queue.pos1, queue.pos2});
            // Two possible cases:
            if (text1.at(queue.pos1) == text2.at(queue.pos2)) {
                // Case character is same at text1[pos1] and text2[pos2], then we take it. There's no reason for us to skip.
                queues.push_back({queue.pos1 + 1, queue.pos2 + 1, queue.cur_score + 1});
            } else {
                // Case character is different. We have either skip a character from text1, or skip a character from text2.
                queues.push_back({queue.pos1 + 1, queue.pos2, queue.cur_score});
                queues.push_back({queue.pos1, queue.pos2 + 1, queue.cur_score});
            }
        }
        return answer;
    }
};


int main() {
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("abcde", "ace") << std::endl;                            // ans: 3 ("ace")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("ezupkr", "ubmrapg") << std::endl;                       // ans: 2 ("ur")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("hofubmnylkra", "pqhgxgdofcvmr") << std::endl;           // ans: 5 ("hofmr")
    }
    {
        Solution solution = Solution();
        std::cout << solution.longestCommonSubsequence("fcvafurqjylclorwfoladwfqzkbebslwnmpmlkbezkxoncvwhstwzwpqxqtyxozkpgtgtsjobujezgrkvevklmludgtyrmjaxyputqbyxqvupojutsjwlwluzsbmvyxifqtglwvcnkfsfglwjwrmtyxmdgjifyjwrsnenuvsdedsbqdovwzsdghclcdexmtsbexwrszihcpibwpidixmpmxshwzmjgtadmtkxqfkrsdqjcrmxkbkfoncrcvoxuvcdytajgfwrcxivixanuzerebuzklyhezevonqdsrkzetsrgfgxibqpmfuxcrinetyzkvudghgrytsvwzkjulmhanankxqfihenuhmfsfkfepibkjmzybmlkzozmluvybyzsleludsxkpinizoraxonmhwtkfkhudizepyzijafqlepcbihofepmjqtgrsxorunshgpazovuhktatmlcfklafivivefyfubunszyvarcrkpsnglkduzaxqrerkvcnmrurkhkpargvcxefovwtapedaluhclmzynebczodwropwdenqxmrutuhehadyfspcpuxyzodifqdqzgbwhodcjonypyjwbwxepcpujerkrelunstebopkncdazexsbezmhynizsvarafwfmnclerafejgnizcbsrcvcnwrolofyzulcxaxqjqzunedidulspslebifinqrchyvapkzmzwbwjgbyrqhqpolwjijmzyduzerqnadapudmrazmzadstozytonuzarizszubkzkhenaxivytmjqjgvgzwpgxefatetoncjgjsdilmvgtgpgbibexwnexstipkjylalqnupexytkradwxmlmhsnmzuxcdkfkxyfgrmfqtajatgjctenqhkvyrgvapctqtyrufcdobibizihuhsrsterozotytubefutaxcjarknynetipehoduxyjstufwvkvwvwnuletybmrczgtmxctuny", "nohgdazargvalupetizezqpklktojqtqdivcpsfgjopaxwbkvujilqbclehulatshehmjqhyfkpcfwxovajkvankjkvevgdovazmbgtqfwvejczsnmbchkdibstklkxarwjqbqxwvixavkhylqvghqpifijohudenozotejoxavkfkzcdqnoxydynavwdylwhatslyrwlejwdwrmpevmtwpahatwlaxmjmdgrebmfyngdcbmbgjcvqpcbadujkxaxujudmbejcrevuvcdobolcbstifedcvmngnqhudixgzktcdqngxmruhcxqxypwhahobudelivgvynefkjqdyvalmvudcdivmhghqrelurodwdsvuzmjixgdexonwjczghalsjopixsrwjixuzmjgxydqnipelgrivkzkxgjchibgnqbknstspujwdydszohqjsfuzstyjgnwhsrebmlwzkzijgnmnczmrehspihspyfedabotwvwxwpspypctizyhcxypqzctwlspszonsrmnyvmhsvqtkbyhmhwjmvazaviruzqxmbczaxmtqjexmdudypovkjklynktahupanujylylgrajozobsbwpwtohkfsxeverqxylwdwtojoxydepybavwhgdehafurqtcxqhuhkdwxkdojipolctcvcrsvczcxedglgrejerqdgrsvsxgjodajatsnixutihwpivihadqdotsvyrkxehodybapwlsjexixgponcxifijchejoxgxebmbclczqvkfuzgxsbshqvgfcraxytaxeviryhexmvqjybizivyjanwxmpojgxgbyhcruvqpafwjslkbohqlknkdqjixsfsdurgbsvclmrcrcnulinqvcdqhcvwdaxgvafwravunurqvizqtozuxinytafopmhchmxsxgfanetmdcjalmrolejidylkjktunqhkxchyjmpkvsfgnybsjedmzkrkhwryzan") << std::endl;
                                                                                                                // ans: 323
    }
    return 0;
}
