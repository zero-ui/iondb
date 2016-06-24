#include <Dictionary.h>

void setup()
{
    /* Begin sketch */
    Serial.begin(9600);

    /* We make this call because BppTree dictionary structure is file-based */
    SD.begin();

    Serial.println("Set-up complete.");

    /* Create Bpp Tree dictionary instance: key type, key size, value size */
    Dictionary<int> *btree = new BppTree<int>(key_type_numeric_signed, sizeof(int), 10);

    /* Insert into bpp tree */
    Serial.println("Insert 3 [\"hello\"]");
    int key_1 = 3;
    string value = "hello!";
    btree->insert(key_1, value);

    /* Return the value inserted into the bpp tree */
    string returned_value = btree->get<string>(key_1);
    Serial.print("Retrieve key 3 and got back: ");
    Serial.println(returned_value);

    Serial.println("Updating value of key 3 to hi!");

    /* Update value */
    string new_value = "hi!";
    btree->update(key_1, new_value);

    /* Verify update was successful */
    returned_value = btree->get<string>(key_1);
    Serial.print("Retrieve key 3 after update and got back: ");
    Serial.println(returned_value);

    /* Delete key and value previously inserted */
    Serial.println("Delete key 3");
    btree->deleteRecord(key_1);

    /* Verify delete was successful */
    Serial.println("Try and retrieve key 3 after deletion");
    returned_value = btree->get<string>(key_1);
    Serial.print("Retrieve key 3 and got back ");
    Serial.println(returned_value);
    /* Returned value was nonsense characters, therefore we know the deletion was successful */

    /* Insert 3 key-value pairs to later test cursor queries */
    Serial.println("Insert 3 [\"test1!\"]");
    string value_1 = "test1!";
    btree->insert(key_1, value_1);

    Serial.println("Insert 3 [\"test2!\"]");
    string value_2 = "test2!";
    btree->insert(key_1, value_2);

    Serial.println("Insert 4 [\"test3!\"]");
    int key_2 = 4;
    string value_3 = "test3!";
    btree->insert(key_2, value_3);

    /* Perform equality query on the key of 3 */
    Serial.println("Testing equality query on key 3: ");
    dict_cursor_t *cursor = btree->equality(key_1);
    ion_record_t record = btree->getRecord(cursor);
    int returned_key = *((int*)record.key);

    /* First record returned */
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Second record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Third record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);
    /* No value is returned, therefore we know there are no further records to search for */

    /* Perform an all records query */
    Serial.println("Testing all records query: ");
    cursor = btree->allRecords();
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);

    /* First record returned */
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Second record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Third record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Fourth record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);
    /* No value is returned, therefore we know there are no further records to search for */

    /* Perform range query on keys k for 2<=k<=3 */
    Serial.println("Testing range query: 2<=key<=3 ");
    cursor = btree->range(2, 3);
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);

    /* First record returned */
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Second record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);

    /* Third record returned */
    record = btree->getRecord(cursor);
    returned_key = *((int*)record.key);
    Serial.print("Key: ");
    Serial.print(returned_key);
    Serial.print(" Value: ");
    Serial.println(record.value);
    /* No value is returned, therefore we know there are no further records to search for */

    /* All methods have been tested */
    Serial.println("End BppTree (int key) test.");

    btree->destroy();
    delete btree;
}


void loop() {}