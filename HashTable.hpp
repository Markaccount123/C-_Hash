#pragma once
#include<iostream>
#include<vector>
using namespace std;
//闭散列
namespace Close
{

	enum State
	{
		EMPTY,
		EXIST,
		DELETE
	};

	template<class T>
	struct HashNode
	{
		State _state = EMPTY;
		T _t;
	};

	template<class K>
	struct Hash
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};

	//模板特化
	template<>
	struct Hash < string >
	{
		size_t operator()(const string& s)
		{
			size_t hash = 0;
			for (auto ch : s)
			{
				//hash += ch;
				hash = hash * 131 + ch; //
			}
			return hash;
		}
	};
	//这里和使用红黑树来封装map和set是一样的，都是由第二个模板参数来控制存储的类型是K,还是pair<const K,V>
	template<class K, class T, class HashFunc = Hash<K>>//需要把这里的K转换为整形
	class HashTable
	{
	public:
		bool Insert(const T& t)
		{
			//载荷因子= 填入表中的元素个数 / 散列表的长度
			//负载因子>0.7(严格控制在0.7以下)
			//只有在这种情况下：效率相对较高，并且空间利用率也较好
			if (_tables.size() == 0 || _size * 10 / _tables.size() == 7)
			{
				size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				//但是这里是不对的，因为有可能原来冲突的，由于空间的扩容，而不再是冲突的了，
				//所以我们应该开辟好一段空间，然后重新对上面的数据进行计算，然后放入这段空间内，在释放原来的空间


				//vector<HashNode<T>> newtables;
				//newtables.resize(newsize);

				////此时就是把原空间上的数据，拿来重新计算放到相对应的新空间上
				//for (size_t i = 0; i < _tables.size(); ++i)
				//{
				//	if (_tables[i]._state == EXIST)
				//	{
				//		//线性探测找在新表中的位置
				//	}
				//}
				//newtables.swap(_tables);

				HashTable<K, T, HashFunc> newht;
				newht._tables.resize(newsize);
				for (auto& e : _tables)
				{
					if (e._state == EXIST)
					{
						//重新计算位置，然后放在这个新的表中
						newht.Insert(e._t); //拿旧空间的数据重新计算我嫌弃太麻烦，我还不如直接重新来一遍插入呢
					}
				}
				_tables.swap(newht._tables);
			}
			//不允许数据冗余
			HashNode<T>*  ret = Find(t);
			if (ret)
				return false;

			HashFunc hf;
			//这里在一开始的时候，你的哈希表里面是没有元素的，所以会出现模0，出错情况
			size_t start = hf(t) % _tables.size();
			size_t index = start; //这样写的好处就是，能够在二次探测的时候方便修改
			//线性探测,找一个空位置
			size_t i = 1;
			while (_tables[index]._state == EXIST) //应该去找下一个空位置
			{
				index = start + i;
				index %= _tables.size();
				++i;
			}
			//跳出循环就是两种情况1.EMPTY  2. DELETE这两种情况都可以把值放进去
			_tables[index]._t = t;
			_tables[index]._state = EXIST;
			_size++;

			return true;
		}

		HashNode<T>* Find(const K& key)
		{
			HashFunc hf;
			size_t start = hf(key) % _tables.size();
			size_t index = start;
			size_t i = 1;
			while (_tables[index]._state != EMPTY)
			{
				if (_tables[index]._t == key && _tables[index]._state == EXIST) //因为可能找到的这个值被删除掉了
				{
					return &_tables[index];
				}
				index = start + i;
				index %= _tables.size();
				++i;
			}
			return nullptr;
		}

		bool Erase(const K& key)
		{
			HashNode<T>* ret = Find(key);
			if (ret == nullptr)
			{
				return false;
			}
			else
			{
				//伪删除
				ret->_state = DELETE;
				return true;
			}
		}
	private:
		vector<HashNode<T>> _tables;
		size_t _size = 0; //有效数据的个数
	};

	void testHashTable()
	{
		HashTable<int, int> ht;
		ht.Insert(5);
		ht.Insert(15);
		ht.Insert(16);
		ht.Insert(17);
		ht.Insert(25);
		ht.Insert(35);
		ht.Insert(45);
		ht.Insert(55);

		//只要K的类型不支持取模，就要写仿函数
		Hash<string> strht;
		cout << strht("abcd") << endl;
		cout << strht("acdb") << endl;
		cout << strht("abdc") << endl;
		cout << strht("adbc") << endl;
	}
}

namespace Open
{

};