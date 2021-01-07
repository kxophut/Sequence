#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "DynamicArray.h"
#include "LinkedList.h"

///Итератор динамического массива в классе Sequence
template <typename T>
class ArraySeqIterator: public Iterator<T>{
private:
    ArrayIterator<T>* items;
public:

    ArraySeqIterator<T>(T* curr, int _max, int _index){
        items = new ArrayIterator<T>(curr, _max, _index);
    }

    ArraySeqIterator(const ArraySeqIterator<T>& iter){
        items = iter.items;
    }

    bool Next() override{
        return this->items->Next();
    };
    bool Prev() override{
        return this->items->Prev();
    };
    T GetData() override{
        return this->items->GetData();
    };
    bool isEqual(const T& item) {
        return this->items->isEqual(item);
    };
    bool isValuable() override{
        return this->items->isValuable();
    };
};


///Абстрактный класс, объединяющий массив и список
template<typename T>
class Sequence : public Iterable<T>{
private:
    friend class Iterator<T>;
public:
    Sequence() = default;

    Sequence(const Sequence<T> &seq) = default;

    ~Sequence() = default;

    virtual T GetFirst () = 0;

    virtual T GetLast () = 0;

    virtual T Get (const int &index) = 0;

    virtual void Set(const int &index, const T &data) = 0;

    bool isEmpty() {
        if (this->GetLength() < 0) throw Exception(3);
        if (this->GetLength() == 0)
            return true;
        else
            return false;
    }

    virtual int GetLength() = 0;

    virtual void Append (const T &item) = 0;

    virtual T Remove(int index) = 0;

    virtual void Prepend (const T &item) = 0;

    virtual void InsertAt (const T &item, const int &index) = 0;

    void Replace (T* arr, const int &size){
        for(int i = 0; i < size; i++)
            this->Set(i, arr[i]);
    }

    virtual void Delete() = 0;

    void Swap (const int &startIndex, const int &endIndex)  {
        if ( startIndex < 0 || endIndex >= this->GetLength() || endIndex < 0 || startIndex >= this->GetLength()) throw Exception(1);
        T copy = this->Get(endIndex);
        this->Set(endIndex,this->Get(startIndex));
        this->Set(startIndex, copy);
    }

    virtual Sequence<T>* GetSubSequence(const int &startIndex, const int &endIndex) = 0;

    virtual Iterator<T>* begin() = 0;

    virtual Iterator<T>* end() = 0;

    virtual std::string String() = 0;
};

///Класс динамического массива в классе Sequence
template<typename T>
class ArraySequence: public Sequence<T>{
private:
    DynamicArray<T>* items;

    friend ArraySeqIterator<T>;
public:
    ArraySequence() {
        this->items = new DynamicArray<T>();
    }

    ArraySequence(T* items_, const int &count){
        this->items = new DynamicArray<T>(items_,count);
    }

    ArraySequence(const DynamicArray<T> &arr){
        this->items = new DynamicArray<T>(arr);
    }

    ArraySequence(const ArraySequence<T> &arr){
        items = new DynamicArray<T>(*(arr.items));
    }

    ArraySequence(const int &_count){
        this->items = new DynamicArray<T>(_count);
    }

    ~ArraySequence(){
        delete items;
    }

    DynamicArray<T>* GetItems() { return items;}

    T Remove (int index) override{
        if(index<items->GetReal() && index>=0) {
            T res = items->Get(index);
            for (int i = index; i < items->GetReal() - 1; i++) {
                items->Set(i, items->Get(i + 1));
            }
            items->Resize(items->GetReal()-1);
            return res;
        }
        else throw Exception(2);
    }

    void Set (const int &index, const T &data) override{
        this->items->Set(index, data);
    }

    T GetFirst () override {
        if (this->items->GetHead() != nullptr)
            return *(this->items->GetHead());
        else
            throw Exception(2);
    }

    T GetLast() override{
        if (this->items->GetHead() != nullptr)
            return *(this->items->GetHead() + this->items->GetReal() - 1);
        else
            throw Exception(2);
    }

    T Get (const int &index) override {
        return this->items->Get(index);
    }

    std::string String() override {
        return this->items->String();
    }

    int GetLength() override {
        return this->items->GetReal();
    };

    void Append  (const T &item) override{
        if (this->items->GetReal() == this->items->GetSize())
            this->items->Resize(this->items->GetSize() + 1);
        if (this->items->GetReal() != this->items->GetSize()) {
            this->items->SetReal(this->items->GetReal()+1);
            for (int i = this->items->GetReal(); i > 1; i--)
                *(this->items->GetHead() + i) = *(this->items->GetHead() + i - 1);
            *(this->items->GetHead()) = item;
        }
        else Exception(4);
    };

    void Prepend (const T &item) override {
        if (this->items->GetReal() == this->items->GetSize())
            this->items->Resize(this->items->GetSize() + 1);
        if (this->items->GetReal() != this->items->GetSize()) {
            items->SetReal(items->GetReal()+1);
            items->Set(items->GetReal()-1, item);
        }
        else printf("Not enough memory to prepend");
    }

    void InsertAt(const T &item, const int &index) override {
        if (index > 0 && index < this->items->GetReal()) {
            if (this->items->GetReal() == this->items->GetSize())
                this->items->Resize(this->items->GetSize() + 1);
            for (int i = this->items->GetReal(); i > index; i--)
                *(this->items->GetHead() + i) = *(this->items->GetHead() + i - 1);
            this->items->SetReal(this->items->GetReal() + 1);
            this->items->Set(index, item);
        } else throw Exception(1);
    };


    ArraySequence<T>* Concat(ArraySequence<T> *arr)  {
        return new ArraySequence<T>(*(this->items->Concat(arr->items)));
    }
    //Конкатенация динамических массивов

    ArraySequence<T>* GetSubSequence(const int &startIndex, const int &endIndex) override {
        if (endIndex >= startIndex && startIndex >= 0 && endIndex < this->GetLength() ) {
            ArraySequence<T>* result = new ArraySequence<T>();
            for (int i = 0; i < (endIndex - startIndex + 1); i++) {
                result->items->Prepend(this->Get(startIndex+i));
            }
            return result;
        } else {
            throw Exception(1);
        }
    }

    ArraySeqIterator<T>* begin() override
    {
        return (new ArraySeqIterator<T>(this->items->GetHead(),  this->items->GetReal()-1,0));
    }

    ArraySeqIterator<T>* end() override
    {
        return (new ArraySeqIterator<T>(this->items->GetHead()+this->items->GetReal() - 1,
                                        this->items->GetReal() - 1, this->items->GetReal() - 1));
    }

    void Delete() override{
        delete items;
    }

};


///Итератор списка в классе Sequence
template <typename T>
class ListSeqIterator: public Iterator<T>{
private:
    ListIterator<T>* items;
public:
    ListSeqIterator(Node<T>* curr){
        items = new ListIterator<T>(curr);
    }

    bool Next() override{
        return this->items->Next();
    };

    bool Prev() override{
        return this->items->Prev();
    };

    T GetData() override{
        return this->items->GetData();
    };

    bool isEqual(const T& item) {
        return this->items->isEqual(item);
    };

    bool isValuable() override{
        return this->items->isValuable();
    };
};

///Класс списка в классе Sequence
template <class T>
class ListSequence: public Sequence<T>{
private:
    LinkedList<T>* items;

    friend ListSeqIterator<T>;
public:

    ListSequence(){
        this->items = new LinkedList<T>;
    }

    ListSequence( const LinkedList<T> &list1){
        auto *list2 = new LinkedList<T>(list1);
        this->items = list2;
    }

    ListSequence(const T* items, const int &count){
        auto *list = new LinkedList<T>(items, count);
        this->items = list;
    }

    T GetFirst () override{
        return this->items->GetFirst();
    };

    T GetLast () override{
        return this->items->GetLast();
    };

    T Get (const int &index)override {
        return this->items->Get(index);
    };

    T Remove(int index) override{
        if(index<items->GetLength()) {
            Node<T> *node = items->getHead();
            for (int i = 0; i < index; i++) node = node->GetNext();
            T res = node->GetData();
            delete node;
            return res;
        }
    }

    ListSequence<T>* GetSubSequence(const int &startIndex, const int &endIndex) override {
        LinkedList<T>* listResult = this->items->GetSubList(startIndex, endIndex);
        ListSequence<T>* seqResult = new ListSequence<T>(*listResult);
        return seqResult;
    };

    int GetLength()override {
        return this->items->GetLength();
    };


    void Append (const T &item)override {
        this->items->Append(item);
    };

    void Prepend (const T &item)override {
        this->items->Prepend(item);
    };

    void InsertAt (const T &item, const int &index)override {
        this->items->InsertAt(item,index);
    };

    ListSequence<T>* Concat(const ListSequence<T> *seq) {
        auto* res = new ListSequence<T>(*(this->items->Concat(seq->items)));
        return res;
    };

    std::string String() override {
        return this->items->String();
    }

    void Set(const int &index, const T &value) override {
        this->items->Set(value, index);
    }

    void Delete() override {
        items->Delete();
    }

    ListSeqIterator<T>* begin() override{
        return (new ListSeqIterator<T>(this->items->getHead()));
    };

    ListSeqIterator<T>* end() override{
        return (new ListSeqIterator<T>(this->items->getTail()));
    }
};
#endif
