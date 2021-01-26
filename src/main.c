#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sha.h"
#include "mbedtls/md.h"

byte shaResult[32];

struct mt_node{
    byte *hashValue;
    struct mt_node *left_node;
    struct mt_node *right_node;
};

struct mt_leaf
{
    char* id;
    byte* data;
    struct mt_node* my_node;
    struct mt_leaf* next;
};

void newNode(char* val, struct mt_node* node){
    // hash of the value
   
    hash(val, shaResult);

    node->hashValue = shaResult;

    node->left_node = NULL;
    node->right_node = NULL;
}

void print_tree(struct mt_leaf* ptr) {
    while (ptr != NULL) { 
        printf("%s\n", ptr->id);
        for(int i=0; i < sizeof(shaResult); i++) {
            printf("%02x", (int)(ptr->data[i]));
        }
        printf("\n");
        ptr = ptr->next; 
    } 
}

void build_tree(char* data[]) {

    struct mt_leaf* head = (struct mt_leaf*)malloc(sizeof(struct mt_leaf));
    struct mt_leaf* second = (struct mt_leaf*)malloc(sizeof(struct mt_leaf));
    struct mt_leaf* third = (struct mt_leaf*)malloc(sizeof(struct mt_leaf));

    struct mt_node* head_leaf_node = (struct mt_node*)malloc(sizeof(struct mt_node));
    newNode(data[0], head_leaf_node);
    head->my_node = head_leaf_node;
    head->id = data[0];
    head->data = head_leaf_node->hashValue;
    
    head->next = second;

    struct mt_node* second_leaf_node = (struct mt_node*)malloc(sizeof(struct mt_node));
    newNode(data[1], second_leaf_node);
    second->my_node = second_leaf_node;
    second->id = data[1];
    second->data = second_leaf_node->hashValue;
    
    second->next = third;

    struct mt_node *third_leaf_node = (struct mt_node*)malloc(sizeof(struct mt_node));
    newNode(data[2], third_leaf_node);
    third->my_node = third_leaf_node;
    third->id = data[2];
    third->data = third_leaf_node->hashValue;
    
    third->next = NULL;

    print_tree(head);
}


void hello_task(void *pvParameter)
{
    printf("Hello world!\n");
    printf("I am starting\n");
    char* data[5] = {"A","B","C","D","E"};
    build_tree(data);
    printf("I am ending and dying\n");
    
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    // esp_restart();
}

void app_main() {
    spi_flash_init();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
}

