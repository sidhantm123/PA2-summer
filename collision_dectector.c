#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct KDNode {
    Point point;
    struct KDNode *left;
    struct KDNode *right;
} KDNode;

// Function to create a new k-d tree node
KDNode* createNode(Point point) {
    KDNode* node = (KDNode*)malloc(sizeof(KDNode));
    node->point = point;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to insert a new point into the k-d tree
KDNode* insert(KDNode* root, Point point, unsigned depth) {
    if (root == NULL) {
        return createNode(point);
    }

    unsigned cd = depth % 2;
    if (cd == 0) {
        if (point.x < root->point.x) {
            root->left = insert(root->left, point, depth + 1);
        } else {
            root->right = insert(root->right, point, depth + 1);
        }
    } else {
        if (point.y < root->point.y) {
            root->left = insert(root->left, point, depth + 1);
        } else {
            root->right = insert(root->right, point, depth + 1);
        }
    }

    return root;
}

// Function to check if a point is within the radius of the circle
int isWithinRadius(Point point, int cx, int cy, int radiusSquared) {
    int dx = point.x - cx;
    int dy = point.y - cy;
    return (dx * dx + dy * dy) <= radiusSquared;
}

// Function to search for points within a given radius in the k-d tree
void rangeSearch(KDNode* root, int cx, int cy, int radiusSquared, unsigned depth, int* count) {
    if (root == NULL) {
        return;
    }

    if (isWithinRadius(root->point, cx, cy, radiusSquared)) {
        (*count)++;
    }

    unsigned cd = depth % 2;
    int pointCoord, circleCoord;
    if (cd == 0) {
        pointCoord = root->point.x;
        circleCoord = cx;
    } else {
        pointCoord = root->point.y;
        circleCoord = cy;
    }

    int diff = pointCoord - circleCoord;
    int diffSquared = diff * diff;

    if (diff > 0) {
        rangeSearch(root->left, cx, cy, radiusSquared, depth + 1, count);
        if (diffSquared <= radiusSquared) {
            rangeSearch(root->right, cx, cy, radiusSquared, depth + 1, count);
        }
    } else {
        rangeSearch(root->right, cx, cy, radiusSquared, depth + 1, count);
        if (diffSquared <= radiusSquared) {
            rangeSearch(root->left, cx, cy, radiusSquared, depth + 1, count);
        }
    }
}

// Function to load points from a file
KDNode* load_points(const char* filename, int* num_points) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    KDNode* root = NULL;
    Point point;
    *num_points = 0;
    while (fscanf(file, "%d %d", &point.x, &point.y) == 2) {
        root = insert(root, point, 0);
        (*num_points)++;
    }

    fclose(file);
    return root;
}

// Function to free the memory allocated for the k-d tree
void freeKDTree(KDNode* root) {
    if (root == NULL) {
        return;
    }
    freeKDTree(root->left);
    freeKDTree(root->right);
    free(root);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <points_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_points;
    KDNode* root = load_points(argv[1], &num_points);

    char query[100];
    while (1) {
        if (!fgets(query, sizeof(query), stdin)) {
            break;
        }

        int cx, cy, radius;
        if (sscanf(query, "%d %d %d", &cx, &cy, &radius) != 3) {
            fprintf(stderr, "Invalid input. Please enter three integers separated by spaces.\n");
            continue;
        }

        int count = 0;
        int radiusSquared = radius * radius;
        rangeSearch(root, cx, cy, radiusSquared, 0, &count);
        printf("%d\n", count);
    }

    // Free the memory allocated for the k-d tree
    freeKDTree(root);

    return 0;
}
