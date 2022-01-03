#include "quotation_collection.h"

typedef struct node *link;

struct node {
	struct quot_s { 
		float value;
		int total_number;
	} quot;
	datetime_t date;
	link left,right;
	int N;
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
		node->N = 1;
		return node;
	}

	if (datetime_cmp(x, h->date)<=0)
		h->left = insertR(h->left, x);
	else
		h->right = insertR(h->right, x);
	h->N++;
	return h;
}

static void printR(link h, int strategy) {
	if(h == NULL)
		return;
	if (strategy == 0) {
				datetime_print(h->date);
		quotation_print(h->quot);
	}

	printR(h->left,strategy);
	if(strategy == 1) {
		datetime_print(h->date);
		quotation_print(h->quot);
	}
	printR(h->right,strategy);
	if(strategy == 2) {
		datetime_print(h->date);
		quotation_print(h->quot);
	}
}

void quotation_collection_print(quotation_collection_t Q) {
	if(Q->n == 0)
		return;
	printf("Data\t\tValore\t\tNumero\n");

	printR(Q->head,1);
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

static link rotR(link h) {
link x = h->left;
h->left = x->right;
x->right = h;
return x;
}

static link rotL(link h) {
link x = h->right;
h->right = x->left;
x->left = h;
return x;
}

static link partR(link h, int r) {
  if(h == NULL || (h->left == NULL && h->right == NULL)) return h;
  int t;
  if(h->left != NULL)
  	t = h->left->N;
  else
  	t = 0;
  if ( t > r) {
    h->left = partR(h->left, r);
    h = rotR(h);
  }
  if ( t < r) {
    h->right = partR(h->right, r-t-1);
    h = rotL(h);
  }
  return h;
}

static int longestPathR(link h, int max);
static int shortestPathR(link h, int lenght);



static link balanceR(link h) {
  int r;
  if (h == NULL)// || h->left == NULL || h->right == NULL )
    return h;

  r = (h->N+1)/2-1;
  h = partR(h, r);
  h->left = balanceR(h->left);
  h->right = balanceR(h->right);
  return h;
}

void quotation_collection_balance(quotation_collection_t Q, int soglia) {
	int shortest = shortestPathR(Q->head, 0),
		longest = longestPathR(Q->head, 0);
	if(longest / shortest > soglia) {
		printf("Esecuzione bilanciamento.\n");
		Q->head = balanceR(Q->head);
	}
	else
		printf("Bilanciamento non necessario, rapporto sotto soglia.\n");
}

static int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}
static int min(int a, int b) {
	if (a < b)
		return a;
	return b;
}

static int longestPathR(link h, int massimo) {
	printf("longestPathR:\nh->date = ");
	datetime_print(h->date);
	printf("\nmax = %d\n\n",massimo);
	int l = 0,r = 0;
	if(h == NULL) {
		printf("foglia, returning %d\n",massimo);
		return massimo;
	}

	if(h->left != NULL) {
		l = longestPathR(h->left, massimo + 1);
	}

	if(h->right != NULL) {
		r = longestPathR(h->right, massimo + 1);
	}
printf("returning l%d,r%d, %d\n", l,r,max(l,max(r,massimo)));
	return max(l,max(r,massimo));
}


static int shortestPathR(link h, int lenght) {
	int l = INT_MAX,r = INT_MAX;
	if(h == NULL)
		return lenght - 1;

	if(h->left == NULL && h->right == NULL)
		return lenght;

	if(h->left != NULL)
		l = shortestPathR(h->left, lenght + 1);
	
	if(h->right != NULL)
		r = shortestPathR(h->right, lenght + 1);


	return min(l,r);
}