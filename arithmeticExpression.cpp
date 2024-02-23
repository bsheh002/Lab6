#include "arithmeticExpression.h"
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;

arithmeticExpression::arithmeticExpression(const string &expression) { // constructor
    infixExpression = expression;
}

void arithmeticExpression::buildTree() { //builds a binary tree from infix to postfix
    string postfixExpression = infix_to_postfix();
    stack<TreeNode*> treeStackNode;

    for (char c : postfixExpression) { //loops through postfix and builds a tree through a stack
        if (isalnum(c)) { //checks if c is alpha-numeric
            TreeNode* newNode = new TreeNode(c, 'a');
            treeStackNode.push(newNode);
        } else {
            TreeNode* operand2 = treeStackNode.top();
            treeStackNode.pop();
            TreeNode* operand1 = treeStackNode.top();
            treeStackNode.pop();
            TreeNode* newNode = new TreeNode(c, 'a');
            newNode->right = operand2;
            newNode->left = operand1;
            treeStackNode.push(newNode);
        }
    }
    root = treeStackNode.top();
}
void arithmeticExpression::infix() { //calls upon infix to go through the tree using a stack
    infix(root);
}
void arithmeticExpression::infix(TreeNode *treeStackNode) { //recursively go through tree in an infix matter
    if (treeStackNode) { //checks if it is a valid stack
        if (treeStackNode->left || treeStackNode->right) { //checks if there is a right or left tree node
            cout << "(";
        }
        infix(treeStackNode->left);
        cout << treeStackNode->data;
        infix(treeStackNode->right);
        if (treeStackNode->left || treeStackNode->right) { //checks if there is a right or left tree node
            cout << ")";
        }
    }
}
void arithmeticExpression::prefix() { //calls prefix function to go through tree
    prefix(root);
}
void arithmeticExpression::prefix(TreeNode *treeStackNode) { //recursively goes through tree in a prefix order
    if (treeStackNode) { //checks to see if tree is valid
        cout << treeStackNode->data;
        prefix(treeStackNode->left);
        prefix(treeStackNode->right);
    }
}
void arithmeticExpression::postfix() { //calls upon to go to postfix
    postfix(root);
}
void arithmeticExpression::postfix(TreeNode *treeStackNode) { //recursively goes through tree in a postfix order
    if (treeStackNode) { //checks if tree exists
        postfix(treeStackNode->left);
        postfix(treeStackNode->right);
        cout << treeStackNode->data;
    }
}
int arithmeticExpression::priority(char op){ //checks an operator to give it a specific priority
    int priority = 0;
    if(op == '('){ //checks if operator is (
        priority =  3;
    }
    else if(op == '*' || op == '/'){ //checks if operator is * or /
        priority = 2;
    }
    else if(op == '+' || op == '-'){ //checks if operator is + or -
        priority = 1;
    }
    return priority;
}

string arithmeticExpression::infix_to_postfix(){ //converts infix to postfix
    stack<char> s;
    ostringstream oss;
    char c;
    for(unsigned i = 0; i< infixExpression.size();++i){ //iterates till end of infixExpression
        c = infixExpression.at(i);
        if(c == ' '){ //checks for spaces
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){ //checks to see if it is at the top of stack
                    oss << s.top();
                    s.pop();
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){ //checks the priorities of each the stack top and c
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    while(!s.empty()){ //checks to see that stack is not empty
        oss << s.top();
        s.pop();
    }
    return oss.str();
}

void arithmeticExpression::visualizeTree(const string &outputFilename){ //generates a dot file in order to visualize the binary tree
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){ //checks to see if file opens
        cout<<"Error opening "<< outputFilename<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}
void arithmeticExpression::visualizeTree(ofstream &outFS, TreeNode *treeStackNode) { //recursively goes through tree in order to connect nodes together to make a binary tree and visualize it
    if (treeStackNode) {
        if (treeStackNode->left) {
            outFS << "\"" << treeStackNode->data << "\"" << " -> " << "\"" << treeStackNode->left->data << "\"" << endl;
            visualizeTree(outFS, treeStackNode->left);
        }
        if (treeStackNode->right) {
            outFS << "\"" << treeStackNode->data << "\"" << " -> " << "\"" << treeStackNode->right->data << "\"" << endl;
            visualizeTree(outFS, treeStackNode->right);
        }
    }
}
