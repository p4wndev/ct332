#include<stdio.h>
#include<stack>
#include<queue>
#include<malloc.h>

#define tankcapacity_X 10
#define tankcapacity_Y 5
#define tankcapacity_Z 6
#define goal 8
#define Maxlength 100

using namespace std;
const char* action[]= {"First State", "pourWaterXY", "pourWaterXZ",
                       "pourWaterYX", "pourWaterYZ",
                       "pourWaterZX", "pourWaterZY"};

typedef struct{
	int x;
	int y;
	int z;
}State;

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
	state->z = 0;
}

void print_State(State state){
	printf("\n	X:%d --- Y:%d --- Z:%d", state.x, state.y, state.z);
}


int goalcheck(State state){
	return (state.x == goal || state.y == goal || state.z == goal);
}

int max(int a, int b){
	return a > b ? a : b;
}

int min(int a, int b){
	return a < b ? a : b;
}

int pourWaterXY(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), 0);
		result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
		result->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterXZ(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.z < tankcapacity_Z){
		result->x = max(cur_state.x - (tankcapacity_Z - cur_state.z), 0);
		result->z = min(cur_state.x + cur_state.z, tankcapacity_Z);
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *result){
		if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), 0);
		result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
		result->z = cur_state.z;
		return 1;
	}
	return 0;
}


int pourWaterYZ(State cur_state, State *result){
		if(cur_state.y > 0 && cur_state.z < tankcapacity_Z){
		result->y = max(cur_state.y - (tankcapacity_Z - cur_state.z), 0);
		result->z = min(cur_state.y + cur_state.z, tankcapacity_Z);
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterZX(State cur_state, State *result){
	if(cur_state.z > 0 && cur_state.x < tankcapacity_X){
		result->z = max(cur_state.z - (tankcapacity_X - cur_state.x), 0);
		result->x = min(cur_state.z + cur_state.x, tankcapacity_X);
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterZY(State cur_state, State *result){
		if(cur_state.z > 0 && cur_state.y < tankcapacity_Y){
		result->z = max(cur_state.z - (tankcapacity_Y - cur_state.y), 0);
		result->y = min(cur_state.z + cur_state.y, tankcapacity_Y);
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}


int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1:return pourWaterXY(cur_state, result);
		case 2:return pourWaterXZ(cur_state, result);
		case 3:return pourWaterYX(cur_state, result);
		case 4:return pourWaterYZ(cur_state, result);
		case 5:return pourWaterZX(cur_state, result);
		case 6:return pourWaterZY(cur_state, result);
		default: printf("\nError calls operators");
			return 0;
	}
}
int compareStates(State a, State b){
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

int find_State(State state, stack<Node*> openStack){
	while(!openStack.empty()){
		if(compareStates(openStack.top()->state, state))
			return 1;
		openStack.pop();
	}
	return 0;
}


Node* DFS_Algorithm(State state){
	stack<Node*> Open_DFS;
	stack<Node*> Close_DFS;
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	Open_DFS.push(root);
	while(!Open_DFS.empty()){
		Node* node = Open_DFS.top();
		Open_DFS.pop();
		Close_DFS.push(node);
		if(goalcheck(node->state))
			return node;
		int opt;
		for(opt = 1; opt <= 6; opt++){
			State newstate;
			makeNullState(&newstate);
			if(call_operator(node->state, &newstate, opt)){
				if(find_State(newstate, Close_DFS) || find_State(newstate, Open_DFS))
					continue;
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = opt;
				Open_DFS.push(newNode);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	stack<Node*> stackPrint;
	while(node->Parent != NULL){
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s", no_action, action[stackPrint.top()->no_function]);
		print_State(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}





int main(){
	State cur_state ={10, 0, 0};
	Node* p = DFS_Algorithm(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
