#include "HashTable.h"
#include <stdexcept>

size_t HashTable::hash_function(const KeyType &key) const {
	size_t hash = 0;
	size_t prime = 31;
	for (char c : key) {
		hash = hash * prime + static_cast<size_t>(c);
	}
	return hash % _capacity;
}

HashTable::HashTable(size_t size) noexcept 
	: _capacity(static_cast<int32_t>(size)), _filled(0), table(size) {}

HashTable::~HashTable() {}

void HashTable::insert(const KeyType &key, const ValueType &value) {
	size_t index = hash_function(key);
	auto &bucket = table[index];

	for (auto &pair : bucket) {
		if (pair.first == key) {
			pair.second = value;
			return;
		}
	}
	bucket.emplace_back(key, value);
	++_filled;

	if (getLoadFactor() > 0.75) {
		size_t new_capacity = _capacity * 2;
		std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(new_capacity);

		for (auto &chain : table) {
			for (auto &pair : chain) {
				size_t new_index = 0;
				size_t hash = 0;
				size_t prime = 31;
				for (char c : pair.first) {
					hash = hash * prime + static_cast<size_t>(c);
				}
				new_index = hash % new_capacity;

				new_table[new_index].emplace_back(pair);
			}
		}

		table = std::move(new_table);
		_capacity = static_cast<int32_t>(new_capacity);
	}
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
	size_t index = hash_function(key);
	const auto &bucket = table[index];

	for (const auto &pair : bucket) {
		if (pair.first == key) {
			value = pair.second;
			return true;
		}
	}
	return false;
}

void HashTable::remove(const KeyType &key) {
	size_t index = hash_function(key);
	auto &bucket = table[index];

	for (auto it = bucket.begin(); it != bucket.end(); ++it) {
		if (it->first == key) {
			bucket.erase(it);
			--_filled;
			return;
		}
	}
}


ValueType &HashTable::operator[](const KeyType &key) {
	size_t index = hash_function(key);
	auto &bucket = table[index];

	for (auto &pair : bucket) {
		if (pair.first == key) {
			return pair.second;
		}
	}

	bucket.emplace_back(key, 0.0);
	++_filled;

	if (getLoadFactor() > 0.75) {
      
		size_t new_capacity = _capacity * 2;
		std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(new_capacity);

		for (auto &chain : table) {
			for (auto &pair : chain) {
				size_t new_index = 0;
				size_t hash = 0;
				size_t prime = 31;
				for (char c : pair.first) {
					hash = hash * prime + static_cast<size_t>(c);
				}
				new_index = hash % new_capacity;

				new_table[new_index].emplace_back(pair);
			}
		}

		table = std::move(new_table);
		_capacity = static_cast<int32_t>(new_capacity);
	}

	return bucket.back().second;
}


double HashTable::getLoadFactor() {
	return static_cast<double>(_filled) / static_cast<double>(_capacity);
}

