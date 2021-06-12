#pragma once
#include<iostream>
#include<vector>
using namespace std;
//��ɢ��
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

	//ģ���ػ�
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
	//�����ʹ�ú��������װmap��set��һ���ģ������ɵڶ���ģ����������ƴ洢��������K,����pair<const K,V>
	template<class K, class T, class HashFunc = Hash<K>>//��Ҫ�������Kת��Ϊ����
	class HashTable
	{
	public:
		bool Insert(const T& t)
		{
			//�غ�����= ������е�Ԫ�ظ��� / ɢ�б�ĳ���
			//��������>0.7(�ϸ������0.7����)
			//ֻ������������£�Ч����Խϸߣ����ҿռ�������Ҳ�Ϻ�
			if (_tables.size() == 0 || _size * 10 / _tables.size() == 7)
			{
				size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				//���������ǲ��Եģ���Ϊ�п���ԭ����ͻ�ģ����ڿռ�����ݣ��������ǳ�ͻ���ˣ�
				//��������Ӧ�ÿ��ٺ�һ�οռ䣬Ȼ�����¶���������ݽ��м��㣬Ȼ�������οռ��ڣ����ͷ�ԭ���Ŀռ�


				//vector<HashNode<T>> newtables;
				//newtables.resize(newsize);

				////��ʱ���ǰ�ԭ�ռ��ϵ����ݣ��������¼���ŵ����Ӧ���¿ռ���
				//for (size_t i = 0; i < _tables.size(); ++i)
				//{
				//	if (_tables[i]._state == EXIST)
				//	{
				//		//����̽�������±��е�λ��
				//	}
				//}
				//newtables.swap(_tables);

				HashTable<K, T, HashFunc> newht;
				newht._tables.resize(newsize);
				for (auto& e : _tables)
				{
					if (e._state == EXIST)
					{
						//���¼���λ�ã�Ȼ���������µı���
						newht.Insert(e._t); //�þɿռ���������¼���������̫�鷳���һ�����ֱ��������һ�������
					}
				}
				_tables.swap(newht._tables);
			}
			//��������������
			HashNode<T>*  ret = Find(t);
			if (ret)
				return false;

			HashFunc hf;
			//������һ��ʼ��ʱ����Ĺ�ϣ��������û��Ԫ�صģ����Ի����ģ0���������
			size_t start = hf(t) % _tables.size();
			size_t index = start; //����д�ĺô����ǣ��ܹ��ڶ���̽���ʱ�򷽱��޸�
			//����̽��,��һ����λ��
			size_t i = 1;
			while (_tables[index]._state == EXIST) //Ӧ��ȥ����һ����λ��
			{
				index = start + i;
				index %= _tables.size();
				++i;
			}
			//����ѭ�������������1.EMPTY  2. DELETE��������������԰�ֵ�Ž�ȥ
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
				if (_tables[index]._t == key && _tables[index]._state == EXIST) //��Ϊ�����ҵ������ֵ��ɾ������
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
				//αɾ��
				ret->_state = DELETE;
				return true;
			}
		}
	private:
		vector<HashNode<T>> _tables;
		size_t _size = 0; //��Ч���ݵĸ���
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

		//ֻҪK�����Ͳ�֧��ȡģ����Ҫд�º���
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