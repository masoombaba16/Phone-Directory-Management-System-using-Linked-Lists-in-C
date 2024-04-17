#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Contact {
    char name[50];
    char phone[15];
    struct Contact* next;
};
struct Contact* createContact(char name[], char phone[]) {
    struct Contact* newContact = (struct Contact*)malloc(sizeof(struct Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->next = NULL;
    return newContact;
}
struct Contact* addContact(struct Contact* head, char name[], char phone[]) {
    // Check if the contact with the same name already exists
    struct Contact* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Contact with the same name already exists.\n");
            return head;
        }
        temp = temp->next;
    }
    struct Contact* newContact = createContact(name, phone);
    if (head == NULL) {
        return newContact;
    } else {
        temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newContact;
        return head;
    }
}
struct Contact* searchContact(struct Contact* head, char name[]) {
    struct Contact* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
void displayContacts(struct Contact* head) {
    struct Contact* temp = head;
    while (temp != NULL) {
        printf("%s: %s\n", temp->name, temp->phone);
        temp = temp->next;
    }
}
struct Contact* deleteContact(struct Contact* head, char name[]) {
    if (head == NULL) {
        printf("Phone Directory is empty.\n");
        return NULL;
    }
    struct Contact* temp = head;
    struct Contact* prev = NULL;
    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Contact not found!\n");
        return head;
    }
    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    printf("Contact deleted successfully!\n");
    return head;
}
void updateContact(struct Contact* contact, char name[], char phone[]) {
    strcpy(contact->name, name);
    strcpy(contact->phone, phone);
}
// Helper function to compare contacts for sorting by name
int compareContacts(const void* a, const void* b) {
    return strcmp(((struct Contact*)a)->name, ((struct Contact*)b)->name);
}
void saveContactsToFile(struct Contact* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    // Count the number of contacts
    int numContacts = 0;
    struct Contact* temp = head;
    while (temp != NULL) {
        numContacts++;
        temp = temp->next;
    }
    // Create an array to store the contacts for sorting
    struct Contact** contactsArray = (struct Contact**)malloc(numContacts * sizeof(struct Contact*));
    // Populate the array with contacts
    temp = head;
    for (int i = 0; i < numContacts; i++) {
        contactsArray[i] = temp;
        temp = temp->next;
    }
    // Sort the contacts array by name
    qsort(contactsArray, numContacts, sizeof(struct Contact*), compareContacts);
    // Write the sorted contacts to the file
    for (int i = 0; i < numContacts; i++) {
        fprintf(fp, "%s,%s\n", contactsArray[i]->name, contactsArray[i]->phone);
    }
    fclose(fp);
    free(contactsArray);
    printf("Contacts saved to file '%s' successfully!\n", filename);
}
struct Contact* loadContactsFromFile(struct Contact* head, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file for reading.\n");
        return head;
    }
    char line[100];
    char name[50];
    char phone[15];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%[^,],%s", name, phone) == 2) {
            head = addContact(head, name, phone);
        }
    }
    fclose(fp);
    return head;
}
int main() {
    struct Contact* phoneDirectory = NULL;
    int choice;
    char name[50];
    char phone[15];
    // Load contacts from the file
    phoneDirectory = loadContactsFromFile(phoneDirectory, "contacts.txt");
    printf("Phone Directory Application\n");
    do {
        printf("\n1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Display All Contacts\n");
        printf("4. Update Contact\n");
        printf("5. Delete Contact\n");
        printf("6. Save Contacts to File\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter name: ");
                scanf("%s", name);
                printf("Enter phone number: ");
                scanf("%s", phone);
                phoneDirectory = addContact(phoneDirectory, name, phone);
                printf("Contact added successfully!\n");
                break;
            case 2:
                printf("Enter name to search: ");
                scanf("%s", name);
                struct Contact* result = searchContact(phoneDirectory, name);
                if (result != NULL) {
                    printf("Contact found: %s - %s\n", result->name, result->phone);
                } else {
                    printf("Contact not found!\n");
                }
                break;
            case 3:
                printf("All Contacts:\n");
                displayContacts(phoneDirectory);
                break;
            case 4:
                printf("Enter name to update: ");
                scanf("%s", name);
                struct Contact* updateResult = searchContact(phoneDirectory, name);
                if (updateResult != NULL) {
                    printf("Enter updated name: ");
                    scanf("%s", name);
                    printf("Enter updated phone number: ");
                    scanf("%s", phone);
                    updateContact(updateResult, name, phone);
                    printf("Contact updated successfully!\n");
                } else {
                    printf("Contact not found!\n");
                }
                break;
            case 5:
                printf("Enter name to delete: ");
                scanf("%s", name);
                phoneDirectory = deleteContact(phoneDirectory, name);
                break;
            case 6:
                saveContactsToFile(phoneDirectory, "contacts.txt");
                break;
            case 7:
                printf("Exiting Phone Directory Application.\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);
    struct Contact* temp;
    while (phoneDirectory != NULL) {
        temp = phoneDirectory;
        phoneDirectory = phoneDirectory->next;
        free(temp);
    }
    return 0;
}
