#pragma once

#include <memory>

namespace bintree {
    template <typename T>
    struct TNode: std::enable_shared_from_this<TNode<T>> {
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeWeakPtr = std::weak_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
            return bool(parent);
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
            return parent.lock();
        }

        TNodeConstPtr getParent() const {
            return parent;
        }

        static TNodePtr createLeaf(T v) {
            return TNodePtr(new TNode(v)); // Private constructor call
        }

        static TNodePtr fork(T v, TNodePtr left, TNodePtr right) { // Use TNodePtr to init class fields
            TNodePtr ptr = TNodePtr(new TNode(v, left, right)); // Private constructor call
            setParent(ptr->getLeft(), ptr);
            setParent(ptr->getRight(), ptr);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
            setParent(l, this->shared_from_this()); // Get self shared_ptr
            std::swap(l, left);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, this->shared_from_this()); // Get self shared_ptr
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }
        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        TNodeWeakPtr parent = TNodeWeakPtr(); // Use weak_ptr to handle parent

        TNode(T v)
            : value(v)
        {
        }
        TNode(T v, TNodePtr left, TNodePtr right) // Use TNodePtr to init class fields
            : value(v)
            , left(left)
            , right(right)
        {
        }

        static void setParent(TNodePtr node, TNodeWeakPtr parent) { // Use TNodePtr to init class fields
            if (node)
                node->parent = parent;
        }
    };
}
