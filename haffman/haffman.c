#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

typedef struct symbol {
    wchar_t symbol;
    unsigned long long code;
    int length;
} SYMBOL;

typedef struct node {
    struct symbol *symbol;
    int length;
    struct node *right;
    struct node *left;
} NODE;

typedef struct queue {
    struct node *node;
    struct queue *next;
} QUEUE;

typedef struct bitstream{
    FILE *file;
    int pos;
    unsigned long long bit;
} BITSTREAM;






BITSTREAM * creating_bitstream(FILE *file){
    BITSTREAM *bitstream = (BITSTREAM *)malloc(sizeof(BITSTREAM));
    bitstream->file = file;
    return bitstream;
}

SYMBOL * creating_symbol(wchar_t symbol_char, int length){
    SYMBOL * symbol = (SYMBOL *)malloc(sizeof (SYMBOL));
    symbol->symbol = symbol_char;
    symbol->length = length;
    return symbol;
}

NODE * creating_node(int length){
    NODE * node = (NODE *)malloc(sizeof(NODE));
    node->symbol = NULL;
    node->length = length;
    node->right = NULL;
    node->left = NULL;
    return node;
}

QUEUE * creating_queue(SYMBOL ** symbols, int len){
    QUEUE *first_elem = (QUEUE *)malloc(sizeof(QUEUE));
    QUEUE *queue = first_elem;
    for (int i = 0; i < len; i++){
        queue->node = creating_node((*(symbols + i))->length);
        queue->node->symbol = *(symbols + i);
        (*(symbols + i))->length = 0;
        queue->next = (QUEUE *)malloc(sizeof(QUEUE));
        queue = queue->next;
    }
    return first_elem;
}

SYMBOL ** creating_simbols(FILE *input, int *len){
    fseek(input, 0, SEEK_END);
    long numBytes = ftell(input);
    rewind(input);
    SYMBOL **symbols = (SYMBOL **)malloc(sizeof(SYMBOL *) * (numBytes / sizeof(wchar_t)));
    int symbols_len = 0;

    for (int i = 0; i < (numBytes / sizeof(wchar_t)); i++){
        wchar_t symbol_char = fgetwc(input);
        int in_massive = 0;
        for (int k = 0; k < symbols_len; k++){
            if ((*(symbols + k))->symbol == symbol_char){
                (*(symbols + k))->length++;
                in_massive = 1;
                while (k > 0 && (*(symbols + k))->length >= (*(symbols + k - 1))->length) {
                    SYMBOL *temp = (*(symbols + k));
                    (*(symbols + k)) = (*(symbols + k - 1));
                    (*(symbols + k - 1)) = temp;
                    k--;
                }
                break;
            }
        }
        if (in_massive == 0){
            *(symbols + symbols_len) = creating_symbol(symbol_char, 1);
            symbols_len++;
        }
    }
    *len = symbols_len;
    return symbols;
}

void print_queue(QUEUE * queue){
    while ((queue != NULL) && (queue->node != NULL)){
        printf("^");
        if (queue->node->symbol != NULL)
            printf("%d %d\n", queue->node->symbol->symbol, queue->node->length);
        else
            printf("%d\n", queue->node->length);
        queue = queue->next;
    }
}

void add_code(NODE ** node, unsigned bit){
    if ((*node)->symbol != NULL){
        (*node)->symbol->code = (*node)->symbol->code << 1 | bit;
        (*node)->symbol->length++;
    }
    else{
        add_code(&((*node)->right), bit);
        add_code(&((*node)->left), bit);
    }
}

QUEUE * add_node(QUEUE *queue){
    QUEUE *first_elem = queue;
    NODE **first_node = &queue->node;
    NODE *node = creating_node(INT_MAX);
    NODE **second_node = &(node);
    NODE * new_node = NULL;
    int queue_length = 0;

    // find second node, if it is  
    while ((queue != NULL) && (queue->node != NULL)){
        queue_length++;
        queue = queue->next;
        if ((*second_node)->length >= queue->node->length){
            second_node = &queue->node;
            break;
        }
    }
    if (queue->next->node != NULL)
        queue_length++;
        
    // find min pair
    if (queue_length > 1){
        queue = queue->next;
        while (queue->node != NULL){
            if ((*first_node)->length > queue->node->length){
                first_node = &queue->node;
            }
            else if ((*second_node)->length > queue->node->length){
                second_node = &queue->node;
            }
            queue = queue->next;
        }
        new_node = creating_node((*first_node)->length + (*second_node)->length);
        new_node->left = *first_node;
        new_node->right = *second_node;

        add_code(&new_node->left, (unsigned)0);
        add_code(&new_node->right, (unsigned)1);

        queue = first_elem;

        if (queue->node == *first_node){
            first_elem = queue->next;
        }
        while(queue->node != NULL){
            if(queue->next->node == *first_node)
                queue->next = queue->next->next;
            queue = queue->next;
        }
        queue = first_elem;
        while(queue->node != NULL){
            if(queue->node == *second_node)
                queue->node = new_node;
            
            queue = queue->next;
        }
        
    }
    // create new node
    else{
        queue = first_elem;
        new_node = creating_node(queue->node->length + queue->next->node->length);
        new_node->left = queue->node;
        new_node->right =  queue->next->node;

        add_code(&new_node->left, (unsigned)0);
        add_code(&new_node->right, (unsigned)1);

        first_elem->next = NULL;
        first_elem->node = new_node;
    }
    return first_elem;
}

NODE * creating_tree(QUEUE *queue){
    while (queue->next != NULL){
        queue = add_node(queue);
    }
    return queue->node;
}

void free_tree(NODE * tree){
    if (tree == NULL)
        return;
    free_tree(tree->left);
    free_tree(tree->right);
    free(tree);
}

void print_tree(NODE *node){
    if (node->symbol != NULL){
        printf("%d ", node->symbol->symbol);
        for (int i = 0; i < node->symbol->length; i++){
            printf("%lld", node->symbol->code >> i & 1);
        }
        printf("\n", node->symbol->code);
    }
    else{
        print_tree(node->right);
        print_tree(node->left);
    }
}

void write_bit(BITSTREAM *bitstream, int bit){
    bitstream -> bit = bit | (bitstream->bit << 1);
    bitstream->pos++;
    if (bitstream->pos == 8){
        fwrite(&(bitstream->bit), sizeof(char), 1, bitstream->file);

        bitstream->pos = 0;
        bitstream->bit = 0;
    }
}

void write_symbol(BITSTREAM *bitstream, wchar_t symbol){
    for (int i = 16 - 1; i >= 0; i--){
        int bit = (symbol >> i) & 1;
        write_bit(bitstream, bit);
    }
}

void tree_to_file(NODE *tree, BITSTREAM *bitstream){
    if (tree->left == NULL){
        write_bit(bitstream, 1);
        write_symbol(bitstream, tree->symbol->symbol);
        return;
    }
    write_bit(bitstream, 0);
    tree_to_file(tree->left, bitstream);
    tree_to_file(tree->right, bitstream);
}

void coding_text(wchar_t symbol_char, BITSTREAM *stream, SYMBOL ** symbols){
    int i = 0;
    while ((*(symbols + i))->symbol != symbol_char){
        i++;
    }
    SYMBOL *symbol = (*(symbols + i));
    
    for (int i = 0; i < symbol->length; i++){
        int bit = (symbol->code >> i) & 1;
        write_bit(stream, bit);
    }
}

void compression(FILE *input, FILE *output){
    int len = 0;
    SYMBOL **symbols = creating_simbols(input, &len);
    QUEUE *queue = creating_queue(symbols, len);
    printf("Queue created\n");

    NODE *tree = creating_tree(queue);
    printf("Tree created\n");

    rewind(input);
    BITSTREAM *bitstream = creating_bitstream(output);
    fwrite(&(tree->length), sizeof(int), 1, bitstream->file);
    tree_to_file(tree, bitstream);
    printf("Tree wroted\n");

    fseek(input, 0, SEEK_END);
    long int count_byte = ftell(input);
    rewind(input);
    for (int i = 0; i < count_byte / sizeof(wchar_t); i++){
        if (i % 100000 == 0){
            printf("%d/%d\n", i, count_byte / sizeof(wchar_t));
        }
        wchar_t symbol = fgetwc(input);
        coding_text(symbol, bitstream, symbols);
    }
    free_tree(tree);
    free(symbols);
    bitstream->bit = bitstream->bit << (8 - bitstream->pos);
    fwrite(&(bitstream->bit), sizeof(char), 1, bitstream->file);
    printf("\nCOMPLITED");
}

int read_bit(int  *bit, BITSTREAM * bitstream){
    if (bitstream->pos == 0) {
        if (fread(&(bitstream->bit), sizeof(char), 1, bitstream->file) != 1)
            return 1;
        bitstream->pos = 8;
    }

    bitstream->pos--;
    *bit = (bitstream->bit >> bitstream->pos) & 1;
    return 0;
}

int read_symbol(wchar_t * symbol, BITSTREAM * bitstream){
    *symbol = 0;
    for (int i = 0; i < 16; i++){
        *symbol = *symbol << 1;
        int bit;

        if(read_bit(&bit, bitstream) == 1)
            return 1;
        *symbol = *symbol | bit;
    }
    return 0;
}


NODE * get_tree(BITSTREAM * bitstream){
    int bit;
    if(read_bit(&bit, bitstream) == 1)
        return NULL;
    if (bit == 1){
        wchar_t symbol;
        if(read_symbol(&symbol, bitstream) == 1)
            return NULL;
        NODE *node = creating_node(0);
        node->symbol = creating_symbol(symbol, 0);
        return node;
    }
    NODE * left_node = get_tree(bitstream);
    NODE * right_node = get_tree(bitstream);
    NODE * node = creating_node(0);
    node->left = left_node;
    node->right = right_node;
    return node;
}

void get_symbol(wchar_t * symbol, BITSTREAM * bitstream, NODE * tree){
    NODE * node = tree;
    while (node->left != NULL){
        int bit;
        read_bit(&bit, bitstream);
        if (bit == 0)
            node = node->left;
        else
            node = node->right;
    }
    *symbol = node->symbol->symbol;
}

void decompression(FILE * input, FILE * output){
    BITSTREAM * bitstream = creating_bitstream(input);
    int len;
    fread(&len, sizeof(long), 1, bitstream->file);
    NODE * tree = get_tree(bitstream);

    for (int i = 0; i < len; i++){
        wchar_t symbol;
        get_symbol(&symbol, bitstream, tree);
        fwrite(&symbol, sizeof(wchar_t), 1, output);
    }
    printf("\nCOMPLITED");
    free_tree(tree);
    free(bitstream);
}

int main(int argc, char * argv[]){
    setlocale(LC_ALL, "");

    char * type = argv[1];
    char * input_file = argv[2];
    char * output_file = argv[3];

    FILE * input = fopen(input_file, "rb");
    FILE * output = fopen(output_file, "wb");

    printf("started\n");

    if (strcmp(type, "c") == 0)
        compression(input, output);
    else if (strcmp(type, "d") == 0)
        decompression(input, output);

    fclose(input);
    fclose(output);

    return 0;
}