/******************************************************************************/
/**
@file		Dictionary.h
@author		Dana Klamut
@brief	  Interface describing how user interacts with general dictionaries
			using C++.
*/
/******************************************************************************/

#if !defined(PROJECT_CPP_DICTIONARY_H)
#define PROJECT_CPP_DICTIONARY_H

#include "../dictionary/dictionary.h"
#include "../dictionary/dictionary_types.h"
#include "../dictionary/ion_master_table.h"
#include "../kv_system.h"

template <class K>class Dictionary {
public:

dictionary_handler_t	handler;
dictionary_t			dict;
int						size_k;
int						size_v;

/* BEFORE USE OF A FILE BASED DICTIONARY IMPLEMENTATION (BPP_TREE, FLAT_FILE),
 * SD.begin(...) MUST BE CALLED. */

err_t
initializeDictionary(
	key_type_t	type_key,
	int			key_size,
	int			value_size,
	int			dictionary_size
) {
	/*If dictionary size isn't used by the implementation, what does it default to for the dictionary_create method? */
	err_t err = dictionary_create(&handler, &dict, 0, type_key, key_size, value_size, dictionary_size	/*What value should this default to? */
				);

	size_k	= key_size;
	size_v	= value_size;

	return err;
}

/**
@brief		Insert a value into a dictionary.

@param		key
				The key that identifies @p value.
@param		value
				The value to store under @p key.
@returns	An error message describing the result of the insertion.
*/
template <typename V>err_t
insert(
	K	key,
	V	value
) {
	ion_key_t	ion_key		= (ion_key_t) &key;
	ion_value_t ion_value	= (ion_value_t) &value;

	err_t err				= dictionary_insert(&dict, ion_key, ion_value);

	return err;
}

/**
@brief		Retrieve a value given a key.

@details	A pointer-to-pointer variable must be created to
			pass into the method as an output parameter before
			the method is called.

@param		key
				The key to retrieve the value for.
@param		value
				A pointer to the value byte array to copy data into.
@return		An error message describing the result of the retrieval.
*/
bool
keyFound(
	K key
) {
	ion_key_t	ion_key = (ion_key_t) &key;
	ion_byte_t	ion_value[dict.instance->record.value_size];

	err_t err			= dictionary_get(&dict, ion_key, ion_value);

	if (err == err_ok) {
		return true;
	}
	else {
		return false;
	}
}

template <typename V>boost::optional <V>
get(
	K key
) {
	ion_key_t	ion_key = (ion_key_t) &key;
	ion_byte_t	ion_value[dict.instance->record.value_size];

	err_t err			= dictionary_get(&dict, ion_key, ion_value);

	if (err == err_ok) {
		return *((V *) ion_value);
	}
	else {
		return boost::none;
	}
}

/**
@brief		Delete a value given a key.

@param		key
				The key to be deleted.
@return		An error message describing the result of the deletion.
*/
err_t
deleteRecord(
	K key
) {
	ion_key_t	ion_key = (ion_key_t) &key;
	err_t		err		= dictionary_delete(&dict, ion_key);

	return err;
}

/**
@brief		Update all records with a given key.

@param		key
				The key to identify records for updating.
@param		value
				The value to update records with.
@return	 An error message describing the result of the update.
*/
template <typename V>err_t
update(
	K	key,
	V	value
) {
	ion_key_t	ion_key		= (ion_key_t) &key;
	ion_value_t ion_value	= (ion_value_t) &value;
	err_t		err			= dictionary_update(&dict, ion_key, ion_value);

	return err;
}

/**
@brief	  Destroys dictionary.

@return		An error message describing the total destruction of the dictionary.
 */
err_t
destroy(
) {
	err_t err = dictionary_delete_dictionary(&dict);

	return err;
}

/*Is this to be used by user? */
/**
@brief	  Opens a dictionary, given the desired config.

@param	  config_info
				The configuration of the dictionary to be opened.
@return	 An error message describing the result of of the open.
*/
err_t
open(
	ion_dictionary_config_info_t config_info
) {
	/*Is config info of type ion_dictionary_config_info_t passed directly from user? */
	err_t err = dictionary_open(&handler, &dict, &config_info);

	return err;
}

/**
@brief	  Closes a dictionary.
*/
err_t
close(
) {
	err_t err = dictionary_close(&dict);

	return err;
}

/**
@brief	  Sets up cursor and predicate to perform a range query on a
			dictionary.

@param	  min_value
				The minimum value to be included in the query.
@param	  max_value
				The maximum value to be included in the query.
@returns	An initialized cursor for the particular query.
*/
dict_cursor_t *
range(
	K	min_key,
	K	max_key
) {
	predicate_t		predicate;
	dict_cursor_t	*cursor;

	ion_key_t	ion_min_key = (ion_key_t) &min_key;
	ion_key_t	ion_max_key = (ion_key_t) &max_key;

	dictionary_build_predicate(&predicate, predicate_range, ion_min_key, ion_max_key);

	dictionary_find(&dict, &predicate, &cursor);

	return cursor;
}

/**
@brief	  Sets up cursor and predicate perform an equality query
			on a dictionary for a given key.

@param	  key
				The key used to determine equality.
@returns	An initialized cursor for the particular query.
*/
dict_cursor_t *
equality(
	K key
) {
	predicate_t		predicate;
	dict_cursor_t	*cursor = NULL;

	ion_key_t ion_key		= (ion_key_t) &key;

	dictionary_build_predicate(&predicate, predicate_equality, ion_key);

	dictionary_find(&dict, &predicate, &cursor);

	return cursor;
}

/**
@brief	  Sets up cursor and predicate in order to find all records
			present in the dictionary.

@returns	An initialized cursor for the particular query.
*/
dict_cursor_t *
allRecords(
) {
	predicate_t		predicate;
	dict_cursor_t	*cursor;

	dictionary_build_predicate(&predicate, predicate_all_records);

	dictionary_find(&dict, &predicate, &cursor);

	return cursor;
}

/**
@brief	  Performs query given initialized cursor.

@param	  cursor
				The initialized cursor for the particular query.
@returns	The current record which satisfies the particular query.
*/
ion_record_t
getRecord(
	dict_cursor_t *cursor
) {
	ion_record_t ion_record;

	ion_record.key		= (ion_key_t) malloc((size_t) size_k);
	ion_record.value	= (ion_value_t) malloc((size_t) size_v);

	cursor_status_t cursor_status;

	if ((((cursor_status = cursor->next(cursor, &ion_record)) == cs_cursor_active) || (cursor_status == cs_cursor_initialized)) && (cursor_status != cs_end_of_results)) {
		return ion_record;
	}

	free(ion_record.key);
	free(ion_record.value);
	cursor->destroy(&cursor);
}

err_t
createMasterTable(
) {
	err_t err = ion_init_master_table();

	return err;
}

/*Will user ever need to create a dictionary through the master table? */
err_t
masterTableCreateDictionary(
) {}

/*Is this to be used by user? */
err_t
masterTableLookup(
	unsigned int					id,
	ion_dictionary_config_info_t	*config
) {
	err_t err = ion_lookup_in_master_table(id, config);

	return err;
}

err_t
masterTableOpenDictionary(
	unsigned int id
) {
	err_t err = ion_open_dictionary(&handler, &dict, id);

	return err;
}

err_t
masterTableCloseDictionary(
) {
	err_t err = ion_close_dictionary(&dict);

	return err;
}

err_t
deleteFromMasterTable(
) {
	err_t err = ion_delete_from_master_table(&dict);

	return err;
}

err_t
closeMasterTable(
) {
	err_t err = ion_close_master_table();

	return err;
}

err_t
deleteMasterTable(
) {
	err_t err = ion_delete_master_table();

	return err;
}
};

#endif /* PROJECT_CPP_DICTIONARY_H */
