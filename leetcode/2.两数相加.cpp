/*
 * @lc app=leetcode.cn id=2 lang=cpp
 *
 * [2] 两数相加
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* head=NULL;
        ListNode* p=NULL;
        int a=0;
        while(l1!=NULL&&l2!=NULL)
        {
            int sum=l1->val+l2->val+a;
            if(p==NULL)
            {
                p=new ListNode(sum%10);
            }
            else
            {
            p->next=new ListNode(sum%10);
            p=p->next;
            }

            if(head==NULL)
            {
                head=p;
            }
            a=sum/10;
            l1=l1->next;
            l2=l2->next;
        }

        while(l1!=NULL)
        {
            p->next=new ListNode();
            p->next->val=(l1->val+a)%10;
            a = (l1->val + a) / 10;
            p = p->next; l1 = l1->next;
        }

                // 如果l2还有剩余节点，继续遍历并添加到结果链表
        while (l2 != NULL) {
            p->next = new ListNode();
            p->next->val = (l2->val + a) % 10; 
            a = (l2->val + a) / 10;
            p = p->next; l2 = l2->next;
        }
                // 如果最后还有进位值，添加到最后
        if (a != 0) {
            p->next = new ListNode(a);
        }
        return head;
    }
};
// @lc code=end

