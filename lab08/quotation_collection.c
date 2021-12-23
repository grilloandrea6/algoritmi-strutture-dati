#include "quotation_collection.h"

typedef struct node *link;

struct node {
	struct quot_s { 
		float value;
		int total_number;
	} quot;
	datetime_t date;
	link left,right;
};

struct quotation_collection_s {
	link head;
	int n;
	// int n? che ti serve?
};


static int quotation_cmp(struct quot_s a, struct quot_s b);
static void quotation_calculate(struct quot_s *quot, int n, float v);
static void quotation_print(struct quot_s quot);

static void freeR(link head);
static link insertR(link h, datetime_t x);


quotation_collection_t quotation_collection_init() {
	quotation_collection_t Q = malloc(sizeof(struct quotation_collection_s));
	Q->head = NULL;
	Q->n = 0;
	return Q;
}

static void freeR(link head) {
	if(head == NULL)
		return;

	freeR(head->left);
	freeR(head->right);
	free(head);
}

void quotation_collection_free(quotation_collection_t Q) {
	freeR(Q->head);
	free(Q);
}

static link searchR(link h, datetime_t date) {
	if(h == NULL)
		return NULL;

	if(datetime_cmp(date, h->date) > 0)
		return searchR(h->right, date);

	if(datetime_cmp(date, h->date) < 0)
		return searchR(h->left, date);

	return h;
}

void quotation_collection_search_print(quotation_collection_t Q, datetime_t date) {
	link h = searchR(Q->head,date);

	datetime_print(date);
	printf("\t%f\t%d\n",h->quot.value,h->quot.total_number);
}

static void searchIntervalR(link h,datetime_t start,datetime_t end, float *min, float *max,int *first) {
	if(h == NULL) return;
	if(datetime_cmp(start,h->date) <= 0)
		searchIntervalR(h->left,start,end,min,max,first);

	if(datetime_cmp(start,h->date) <= 0 && datetime_cmp(end,h->date) >= 0) {
		if(*first) {
			*min = *max = h->quot.value;
			*first = 0;
		} else {
			*min = (h->quot.value < *min) ? h->quot.value : *min;
			*max = (h->quot.value > *max) ? h->quot.value : *max;
		}
	}

	if(datetime_cmp(end,h->date) >= 0)
		searchIntervalR(h->right,start,end,min,max,first);
}


void quotation_collection_search_interval_print(quotation_collection_t Q, datetime_t start,datetime_t end) {
	float min,max;
	int first = 1;
	searchIntervalR(Q->head,start,end,&min,&max,&first);
	printf("Quotazione minima = %f\nQuotazione massima = %f\n",min,max);
}


void quotation_collection_read_quotations(quotation_collection_t Q, FILE* fp) {
	int n_quotations,i,num;
	float value;
	datetime_t date;
	link l;

	if(fp == NULL) {
		fprintf(stderr, "file non valido!!\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp,"%d",&n_quotations);
	for(i = 0; i < n_quotations; i++) {
		date = datetime_read(fp);
		l = searchR(Q->head,date);

		if(l == NULL) {
			Q->head = insertR(Q->head,date);			// ToDo check if it can be done better than searching again
			l = searchR(Q->head, date);
		}
		if(l == NULL) {
			fprintf(stderr,"houston, we got a problem!!");
			exit(0);
		}	
		fscanf(fp,"%f %d",&value,&num);
		quotation_calculate(&(l->quot), num,value);
		Q->n++;
	}
}



static link insertR(link h, datetime_t x) {
	if (h == NULL) {
		link node = malloc(sizeof(struct node));
		node->date = x;
		node->quot.total_number = -1;
		node->left = node->right = NULL;
		return node;
	}

	if (datetime_cmp(x, h->date)<=0)
		h->left = insertR(h->left, x);
	else
		h->right = insertR(h->right, x);
	return h;
}

static void printR(link h) {
	if(h == NULL)
		return;

	printR(h->left);
	datetime_print(h->date);
	quotation_print(h->quot);
	printR(h->right);
}

void quotation_collection_print(quotation_collection_t Q) {
	if(Q->n == 0)
		return;
	printf("Data\t\tValore\t\tNumero\n");

	printR(Q->head);
}



static void quotation_calculate(struct quot_s *quot, int n, float v) {
	if(quot->total_number == -1) {
		quot->total_number = n;
		quot->value = v;
		return;
	}

	quot->value = ((quot->value * quot->total_number) + (v * n)) / (n + quot->total_number);
	quot->total_number += n;
}


static void quotation_print(struct quot_s quot) {
	printf("\t%f\t%d\n",quot.value,quot.total_number);
}