#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define BUF_SIZE 80
#define NAME_SIZE 32
struct student {
	int score;
	char name[NAME_SIZE];
};
struct student *table;
int counter_compare, counter_swap;
void Clear() {
	counter_compare = 0; counter_swap = 0;
}
void Counter() {
	printf("Compare %d Swap %d\n", counter_compare, counter_swap);
}
int my_compare(const struct student *a, const struct student *b) {
	counter_compare++;
	return (a->score > b->score) ? 1 : (a->score < b->score) ? -1 : 0;
}
void my_swap(struct student *a, struct student *b) {
	struct student c;
	counter_swap++;
	memcpy(&c, a, sizeof(struct student));
	memcpy(a, b, sizeof(struct student));
	memcpy(b, &c, sizeof(struct student));
}
void Bubble(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	/* Bubble Sort */
	int i, j;
	for (i = 0; i < size - 1; i++)
		for (j = size - 1; j > i; j--)
			/* If table[j-1] is larger than table[j] */
			if (my_compare(&table[j-1], &table[j]) == 1)
				/* Swap table[j-1] and table[j] */
				my_swap(&table[j-1], &table[j]);

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) + 
			(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}
void Select(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	/* Selection Sort */
	int i, j;
	for (i = 0; i < size - 1; i++) {
		int min = i;
		for (j = i + 1; j < size; j++)
			/* If table[min] is larger than table[j] */
			if (my_compare(&table[min], &table[j]) == 1)
				/* Update min with j */
				min = j;
		/* Swap table[i] and table[min] */
		my_swap(&table[i], &table[min]);
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) + 
			(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}
void Insert(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	/* Insertion Sort */
	int i, j;
	for (i = 1; i < size; i++) {
		j = i;
		/* While table[j - 1] is larger than table[j] */
		while (j >= 1 && my_compare(&table[j-1], &table[j]) == 1 /* table[j - 1] is larger than table[j] */) {
			/* Swap table[j] and table[j-1] */
			my_swap(&table[j], &table[j-1]);
			j--;
		}
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) + 
			(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}
void Shell(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	/* Shell Sort */
	int i, j, gap;
	gap = size / 2;
	while (gap > 0) {
		for (i = gap; i < size; i++) {
			/* Begin Insertion Sort */
			j = i;
			while (j >= gap &&
				my_compare(&table[j-gap], &table[j]) == 1) {
				my_swap(&table[j], &table[j-gap]);
				j -= gap;
			}
			/* End Insertion Sort */
		}
		gap = gap / 2;
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) + 
			(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}

void heapify(int n, int i) {
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < n && my_compare(&table[l], &table[largest]) == 1)
		largest = l;
	if (r < n && my_compare(&table[r], &table[largest]) == 1)
		largest = r;

	if (largest != i) {
		my_swap(&table[i], &table[largest]);
		heapify(n, largest);
	}
}

void Heap(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	for (int i = size / 2 - 1; i >= 0; i--)
		heapify(size, i);
	for (int i = size - 1; i >= 0; i--) {
		my_swap(&table[0], &table[i]);
		heapify(i, 0);
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) +
		(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}

int partition(int low, int high) {
	struct student pivot = table[high];
	int i = low - 1;
	for (int j = low; j < high; j++) {
		if (my_compare(&table[j], &pivot) != 1) {
			i++;
			my_swap(&table[i], &table[j]);
		}
	}
	my_swap(&table[i + 1], &table[high]);
	return i + 1;
}

void quick_sort(int low, int high) {
	if (low < high) {
		int pi = partition(low, high);
		quick_sort(low, pi - 1);
		quick_sort(pi + 1, high);
	}
}

void Quick(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	quick_sort(0, size - 1);

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) +
		(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}

void merge(int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;
	struct student *L = malloc(n1 * sizeof(struct student));
	struct student *R = malloc(n2 * sizeof(struct student));
	for (int i = 0; i < n1; i++) memcpy(&L[i], &table[left + i], sizeof(struct student));
	for (int j = 0; j < n2; j++) memcpy(&R[j], &table[mid + 1 + j], sizeof(struct student));

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (my_compare(&L[i], &R[j]) != 1)
			memcpy(&table[k++], &L[i++], sizeof(struct student));
		else
			memcpy(&table[k++], &R[j++], sizeof(struct student)), counter_swap++;
	}
	while (i < n1) memcpy(&table[k++], &L[i++], sizeof(struct student));
	while (j < n2) memcpy(&table[k++], &R[j++], sizeof(struct student));
	free(L); free(R);
}

void merge_sort(int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		merge_sort(left, mid);
		merge_sort(mid + 1, right);
		merge(left, mid, right);
	}
}

void MergeSort(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	merge_sort(0, size - 1);

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) +
		(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}

void Shaker(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	int left = 0, right = size - 1;
	int i;
	while (left < right) {
		for (i = left; i < right; i++) {
			if (my_compare(&table[i], &table[i + 1]) == 1) {
				my_swap(&table[i], &table[i + 1]);
			}
		}
		right--;

		for (i = right; i > left; i--) {
			if (my_compare(&table[i - 1], &table[i]) == 1) {
				my_swap(&table[i - 1], &table[i]);
			}
		}
		left++;
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) +
		(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}

void Comb(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	int gap = size;
	const float shrink = 1.3;
	int sorted = 0;
	while (!sorted) {
		gap = (int)(gap / shrink);
		if (gap <= 1) {
			gap = 1;
			sorted = 1;
		}
		for (int i = 0; i + gap < size; i++) {
			if (my_compare(&table[i], &table[i + gap]) == 1) {
				my_swap(&table[i], &table[i + gap]);
				sorted = 0;
			}
		}
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) +
		(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}

void Gnome(int size) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	int pos = 0;
	while (pos < size) {
		if (pos == 0 || my_compare(&table[pos - 1], &table[pos]) <= 0) {
			pos++;
		} else {
			my_swap(&table[pos], &table[pos - 1]);
			pos--;
		}
	}

	gettimeofday(&etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) +
		(etime.tv_usec - stime.tv_usec) / 1000000.0;
	printf("Elapsed Time %.4f [sec] \n", sec);
}


void Disp(int size) {
	int i;
	for (i = 0; i < size; i++)
		printf("%d\t%s\n", table[i].score, table[i].name);
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int n;
	char buf[BUF_SIZE];
	char command;
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	/* Read the file to count the number of lines in the file */
	n = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL)
		n++;
	/* Memory allocation */
	if ((table = malloc(sizeof(struct student) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Read the file again to copy the data */
	n = 0;
	fseek(fp, 0L, SEEK_SET);
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		sscanf(buf, "%d\t%s", &table[n].score, table[n].name);
		n++;
	}
	fclose(fp);

	Clear();
	printf("[d]       Display Table\n");
	printf("[h]       Shell Sort\n");
	printf("[i]       Insertion Sort\n");
	printf("[s]       Selection Sort\n");
	printf("[b]       Bubble Sort\n");
    printf("[q]       Quick Sort\n");
    printf("[m]       Merge Sort\n");
    printf("[p]       Heap Sort\n");
    printf("[k]       Shaker Sort\n");
    printf("[g]       Gnome Sort\n");
    printf("[o]       Comb Sort\n");
	printf("[c]       Clear Counters\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'h':	Shell(n); Counter(); break;
		case 'i':	Insert(n); Counter(); break;
		case 's':	Select(n); Counter(); break;
		case 'b':	Bubble(n); Counter(); break;
        case 'q':	Quick(n); Counter(); break;
        case 'm':	MergeSort(n); Counter(); break;
        case 'p':	Heap(n); Counter(); break;
        case 'k':	Shaker(n); Counter(); break;
        case 'g':	Gnome(n); Counter(); break;
        case 'o':	Comb(n); Counter(); break;
		case 'c':	Clear(); break;
		case 'e':	free(table); return 0;
		}
	}
	return 0;
}