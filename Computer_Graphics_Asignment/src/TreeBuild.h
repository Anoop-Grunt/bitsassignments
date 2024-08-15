#pragma once
///object of this class are the nodes in the tree
class TreeNode
{
public:
	/// @param info just the value that will be stored in the node, in this case it's a character
	/// @param father the adrress to the father node
	///
	/// the node class is similar to the one in the reingold tilford pascal implementation, but it additionally stores the address of the father
	TreeNode(char info, TreeNode* father);
	/// The destructor destroys the nodes children first then destroys the current object.
	~TreeNode();
	/// @param info the character to which the node value will be changed
	void resetCharacter(char info);
	/// retruns the character fron the node, this will not be plotted and is mainly used for debugging purposes
	char getInfo() const;
	/// @param l address of the left child
	///
	/// modifies the left child pointer to point to the parameter passed
	void addLeftChild(TreeNode* l);
	/// returns the address of the left child
	TreeNode* leftChildAddress() const;
	/// @param r the address of the right child
	///
	/// modifies the right child ointer to the parameter passed
	void addRightChild(TreeNode* r);
	/// returns the address of the right child
	TreeNode* rightChildAddress() const;
	/// @param x the x-co-ordinate
	///
	/// sets the x co ordinate of the current node using the offsets during pre-order traversal after the setup has finished
	void set_x_cood(int x);
	/// returns the x co ordinate of the node
	int x_cood() const;
	/// @param y the y co -ordinate 
	///
	/// sets the value of the y- co ordinate usig level of the node
	void set_y_cood(int y);
	/// returns the y co ordinate of the node
	int y_cood() const;
	/// returns true if the current node is a thread -> this is used when the next element of the contour is not the current nodes son.
	bool isThread() const;
	/// makes the current node a thread(uses the empty pointers)
	void setThread(bool val);
	/// @param val the relative separation between the parent and childs x-co ordinates 
	///
	/// sets the offset to the childrens x-co ordinates 
	void setOffset(int val);
	/// returns the offset value to the children
	int getOffset() const;
	/// @param father the address to the father node
	///
	/// sets the father nodes address to the received parameter
	void setParent(TreeNode* father);
	/// returns the fathers address 
	TreeNode* getParent() const;
	/// used maily for debugging, returns the status value
	int getStatus() const;
	/// @param stat the status bit
	///
	/// this was used while debuggin to check if a threaded nodes child was being visited, this isn't used anywhere in the build version 
	void setStatus(int stat);
	/// returns the mod value, this was used for debugging
	int Mod_Value() const;
	/// the mod value is the same as the offset in the final build, earlier implemenations used the mod value when the relative separations were stored in the children rather than the parent, in that case the x co--ordinate during pre order traversal will be set to (parent->mod + child's relative x position, and child->mod will be modified as child->mod = child -> mod + parent -> mod)
	void setModifier(int modf);



	TreeNode* right;
	TreeNode* left;
	TreeNode* parent;
	char m_info;
	int x;
	int y;
	int status;
	int offset;
	bool thread;
};
/// @param root the address of the root node
/// @param xPos the root nodes x-co-r=ordinate in pixel space, since the windows width is  pixels its recommended that this is always set to 
/// calculates the absolute co -ordinates of the nodes by  doing a pre order traversal and adding up the offsets 
void petrify(TreeNode* root, int xPos);
/// @param root the address to the root node
/// @param minsep the minimus separation between the x co ordinates of sibling nodes
///
/// plots the tree using bresenhams circle drawing algorith for nodes, bresenhams line drawing algorith for the connections, also uses som basic trigonometry to properly incline the connections
void plot_tree(TreeNode* root, int minsep);
/// the structure used in the TR algorithm to store the details of the leftmost and right most values of the lowest level in the countour 
struct Extreme
{
	TreeNode* addr;
	int off;
	int lev;
};