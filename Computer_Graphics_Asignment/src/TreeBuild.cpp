
#include <cmath>
#include "Bresenham_Circle.h"
#include "BresenhamLIne.h"
#include "TreeBuild.h"
#include "glm/glm.hpp"
int MINSEP = 50; ///< the minimum x distance between siblings



TreeNode::TreeNode(char info, TreeNode* father) : m_info(info), left(nullptr), right(nullptr), x(-1), y(-1), offset(0), thread(false), parent(father), status(0) { }



void TreeNode::resetCharacter(char info) { m_info = info; }

char TreeNode::getInfo() const { return m_info; }



void TreeNode::addLeftChild(TreeNode* l) { left = l; }

TreeNode* TreeNode::leftChildAddress() const { return left; }



void TreeNode::addRightChild(TreeNode* r) { right = r; }

TreeNode* TreeNode::rightChildAddress() const { return right; }



void TreeNode::set_x_cood(int x) { this->x = x; }

int TreeNode::x_cood() const { return x; }



void TreeNode::set_y_cood(int y) { this->y = y; }

int TreeNode::y_cood() const { return y; }



bool TreeNode::isThread() const { return thread; }

void TreeNode::setThread(bool val) { thread = val; }



void TreeNode::setOffset(int val) { offset = val; }

int TreeNode::getOffset() const { return offset; }



void TreeNode::setParent(TreeNode* father) { this->parent = father; }

TreeNode* TreeNode::getParent() const { return this->parent; }



int TreeNode::getStatus() const { return this->status; }

void TreeNode::setStatus(int stat) { this->status = stat; }



int TreeNode::Mod_Value() const { return this->offset; }

void TreeNode::setModifier(int modf) { this->offset = modf; }







TreeNode::~TreeNode() {

    if (left != nullptr)

        left->~TreeNode();

    if (right != nullptr)

        right->~TreeNode();

}

/// this is the function from the reingold tilford tidier drawing of trees paper
/// the way the algorithm works is detailed in brief:
/// We start a post order traversal from the root, this is important because we need to visit the children first to properly center the parent above them
/// we then calculate the minimum root separation as  follows : we traverse the subtrees level wise, and in the right subtree we find the left most node, and in the left subtree we find the rightmost node.
/// the set of all the left most nodes in every level of the right subtree is called the righ contour, and similarly theres a left contour for the left subtree(all the rightmost nodes)
/// we check at every level if the separation between these is less than MINSEP we desire
/// if it really is less than minsep then we modify the curr_sep in a way that when we finish scanning the countour we can transfer this currsep to the rootsep, this way the minsep distance is established at every level of the contour
/// since this is very likely an o(n^2) operation we use the concept of threads
/// for this we first calculate loffSum and roffSum which are the absolute separations of the lowest levels leftmost on the right contour, and the lowest levels righmost node on the left contour
/// now before we leave the node we check if we need to modify the rl lr ll or r's level  values:
/// the way we do that is we check if the left most node of the lowest level belongs to the other contour i.e(compare rr , lr and ll , rl)
/// then we update the lmost , rmost levels to the lowest of the above levels
/// Now if the next element on the contour is not a child of the current element, the current element must be a leaf, and the free pointers are used to set threads to the extreme values we just found
/// finally we have the offsets(rootseparations) off all the nodes, so we can petrify the tree to find the absolute co ordinates of the nodes.

 
void setup(TreeNode* root, int level, struct Extreme* rMost, struct Extreme* lMost, int stepY) {

    TreeNode* l, * r;

    struct Extreme ll, lr, rl, rr;



    int currSep, rootSep, lOffSum, rOffSum;

    if (root == nullptr) {

        rMost->lev = -1;

        lMost->lev = -1;

    }

    else {

        root->set_y_cood(level);

        l = root->leftChildAddress();

        r = root->rightChildAddress();

        setup(l, level + 1, &lr, &ll, stepY);
        //above step scans the whole left tree and modifies the lr, ll values in current call
        setup(r, level + 1, &rr, &rl, stepY);
        //above step scans the whole right subtree and modifies rr, and rl in current call
        //so you have the lr, rl, rr, ll values for current node(these values are only really mo)
        if (r == nullptr && l == nullptr) {
            //this means that we are at a leaf
            rMost->addr = root;

            lMost->addr = root;

            rMost->lev = level;

            lMost->lev = level;

            rMost->off = 0;

            lMost->off = 0;

            root->setOffset(0);

        }

        else {

            currSep = MINSEP;

            rootSep = MINSEP;

            lOffSum = 0;

            rOffSum = 0;



            while (l != nullptr && r != nullptr) {

                if (currSep < MINSEP) {

                    rootSep += MINSEP - currSep;

                    currSep = MINSEP;

                }

                if (l->rightChildAddress() != nullptr) {

                    lOffSum += l->getOffset();

                    currSep -= l->getOffset();

                    l = l->rightChildAddress();

                }

                else {

                    lOffSum -= l->getOffset();

                    currSep += l->getOffset();

                    l = l->leftChildAddress();

                }



                if (r->leftChildAddress() != nullptr) {

                    rOffSum -= r->getOffset();

                    currSep -= r->getOffset();

                    r = r->leftChildAddress();

                }

                else {

                    rOffSum += r->getOffset();

                    currSep += r->getOffset();

                    r = r->rightChildAddress();

                }

            } // contour scanning done

            root->setOffset((rootSep + 1) / 2);

            lOffSum -= root->getOffset();

            rOffSum += root->getOffset();



            if (rl.lev > ll.lev || root->leftChildAddress() == nullptr) {

                lMost->addr = rl.addr;

                lMost->lev = rl.lev;

                lMost->off = rl.off;

                lMost->off += root->getOffset();

            }

            else {

                lMost->addr = ll.addr;

                lMost->lev = ll.lev;

                lMost->off = ll.off;

                lMost->off -= root->getOffset();

            }

            if (lr.lev > rr.lev || root->rightChildAddress() == nullptr) {

                rMost->addr = lr.addr;

                rMost->lev = lr.lev;

                rMost->off = lr.off;

                rMost->off -= root->getOffset();

            }

            else {

                rMost->addr = rr.addr;

                rMost->lev = rr.lev;

                rMost->off = rr.off;

                rMost->off += root->getOffset();

            }



            if (l != nullptr && l != root->leftChildAddress()) {

                rr.addr->setThread(true);

                rr.addr->setOffset(abs((rr.off + root->getOffset()) - lOffSum));

                if ((lOffSum - root->getOffset()) <= rr.off) {

                    rr.addr->addLeftChild(l);

                }

                else {

                    rr.addr->addRightChild(l);

                }

            }

            else if (r != nullptr && r != root->rightChildAddress()) {

                ll.addr->setThread(true);

                ll.addr->setOffset(abs((ll.off - root->getOffset()) - rOffSum));

                if ((rOffSum + root->getOffset()) >= ll.off) {

                    ll.addr->addRightChild(r);

                }

                else {

                    ll.addr->addLeftChild(r);

                }

            }

        }

    }

}

 

void petrify(TreeNode* root, int xPos) {

    if (root != nullptr) {

        root->set_x_cood(xPos);
        BresenCircle c(root->x_cood(), 480 - (root->y_cood()+1)*50, 20);
        c.draw2();
        if (root->getParent() != NULL) {
            // co-ordinates of the child node.
            float xc1 = root->x_cood();
            float yc1 = 480 - (root->y_cood() + 1) * 50; 
            // co-ordinates of the parent node.
            float xc2 = root->getParent()->x_cood();
            float yc2 = 480 - (root->getParent()->y_cood() + 1) * 50;
            
            glm::vec2 p1(root->x_cood(), 480 - (root->y_cood() + 1) * 50);
            glm::vec2 p2(root->getParent()->x_cood(), 480 - (root->getParent()->y_cood() + 1) * 50);
            int sign_handler = 1;
            if (root->getParent()->rightChildAddress() == root) { 
                sign_handler = -1;
                /*BresenCircle c(root->x_cood(), 480 - (root->y_cood() + 1) * 50, 10);
                c.draw2();*/
            }
            float angle = glm::atan((yc2- yc1)/(xc2 - xc1));
            float x1 = xc1 + 20*(glm::cos(angle)) * sign_handler;
            float y1 = yc1 + 20*(glm::sin(angle)) * sign_handler;
            float x2 = xc2 - 20*(glm::cos(angle)) * sign_handler;
            float y2 = yc2 - 20*(glm::sin(angle)) * sign_handler;
            BresenLine l(x1, y1, x2, y2);
            l.draw2();

        }
        if (root->isThread()) {

            root->setThread(false);

            root->addLeftChild(nullptr);

            root->addRightChild(nullptr);

        }

        petrify(root->leftChildAddress(), xPos - root->getOffset());

        petrify(root->rightChildAddress(), xPos + root->getOffset());

    }

}



void plot_tree(TreeNode* root, int minsep) {

    MINSEP = minsep;

    struct Extreme rm, lm;

    setup(root, 0, &rm, &lm, minsep);

    petrify(root, 320);



}