#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__
#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

class DecisionTree
{
public:
	DecisionTree();
	~DecisionTree();

	// Getters Setters
	Agent* getAgent() const;
	void setAgent(Agent* agent);

	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void Update();

	std::string MakeDecision();

private:
	Agent* m_agent;
	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;

	void m_buildTree();

	std::vector<TreeNode*> m_treeNodeList;
};


#endif
