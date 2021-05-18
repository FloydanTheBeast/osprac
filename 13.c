#include <unistd.h>
#include <stdio.h>

int main() {
	fopen("test", "w+b");
	symlink("test", "0");
	char linkName[3], nextLinkName[3];
	int linkIndex = 0;
	int recursionDepth = 0;

	sprintf(linkName, "%d", linkIndex);
	while (fopen(linkName, "r") != NULL) {
		recursionDepth++;
		sprintf(linkName, "%d", linkIndex);
		sprintf(nextLinkName, "%d", ++linkIndex);
		symlink(linkName, nextLinkName);
		sprintf(linkName, "%d", linkIndex);
	}

	for (int i = 0; i < recursionDepth; i++) {
		sprintf(linkName, "%d", i);
		remove(linkName);
	}

	printf("Recursion depth is %d\n", recursionDepth);
	return 0;
}