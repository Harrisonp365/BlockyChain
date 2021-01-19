#include <iostream>
#include "Blockchain.h"

using namespace std;

int main() {
	
	Blockchain bChain = Blockchain();

	cout << "Mining block 1...\n" << endl;
	bChain.AddBlock(Block(1, "Block 1 data"));
	cout << "\n\n";

	cout << "Mining block 2...\n" << endl;
	bChain.AddBlock(Block(2, "Block 2 data"));
	cout << "\n\n";

	cout << "Mining block 3...\n" << endl;
	bChain.AddBlock(Block(3, "Block 3 data"));
	cout << "\n\n";

	return 0;
}