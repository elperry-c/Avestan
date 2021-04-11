/// @file meta.hpp
/// �e���v���[�g���^�v���O���~���O.
/// Loki, TTL �̂�������p�N��܂���.
#pragma once

namespace mew {
/// �R���p�C�����Ɍ^������.
namespace meta {
//==============================================================================
// ����.

struct Void;
template <typename T>
struct Void1;

//==============================================================================
/// �ÓI�Ȍ^���.
template <typename T>
struct TypeOf {
  enum {
    is_pointer = 0,    ///< �|�C���^���ۂ�.
    is_reference = 0,  ///< �Q�Ƃ��ۂ�.
  };
  using value_type = T;
  using pointer_type = T*;
  using reference_type = T&;
  using param_type = const T&;
};
template <typename T>
struct TypeOf<T*> {
  enum {
    is_pointer = 1,
    is_reference = 0,
  };
  using value_type = T;
  using pointer_type = T*;
  using reference_type = T&;
  using param_type = T*;
};
template <typename T>
struct TypeOf<T&> {
  enum {
    is_pointer = 0,
    is_reference = 1,
  };
  using value_type = T;
  using pointer_type = T*;
  using reference_type = T&;
  using param_type = T&;
};

//==============================================================================
/// bool If<value, Then, Else>::Result = (if value then Then else Else).
template <bool exp, typename Then, typename Else>
struct If {
  using Result = Then;
};
template </*false*/ typename Then, typename Else>
struct If<false, Then, Else> {
  using Result = Else;
};

//==============================================================================
/// bool Equals<T1, T2>::value = (T1 == T2).
template <typename T1, typename T2>
struct Equals {
  enum { value = 0 };
};
template <typename T>
struct Equals<T, T> {
  enum { value = 1 };
};

//==============================================================================
/// bool Convertable<T1, T2>::value = (if T2 t = T1() is ok).
template <typename T1, typename T2>
struct Convertable {
 private:
  struct U1 {
    char dummy[1];
  };
  struct U2 {
    char dummy[2];
  };
  static U1 test(T2);
  static U2 test(...);
  static T1 make();

 public:
  enum { value = (sizeof(U1) == sizeof(test(make()))) };
};
template <typename T1, typename T2, size_t size>
struct Convertable<T1[size], T2> {
  enum { value = Convertable<T1*, T2>::value };
};
template <typename T>
struct Convertable<T, T> {
  enum { value = 1 };
};
template <typename T>
struct Convertable<void, T> {
  enum { value = 0 };
};
template <typename T>
struct Convertable<T, void> {
  enum { value = 0 };
};
template <>
struct Convertable<void, void> {
  enum { value = 1 };
};

//==============================================================================

template <class T, class U>
struct SuperSubclass {
  enum {
    value =
        (Convertable<const volatile U*, const volatile T*>::value && !Equals<const volatile T*, const volatile void*>::value)
  };
};

#define SUPERSUBCLASS(T, U) mew::meta::SuperSubclass<T, U>::value

//==============================================================================

template <class H, class T>
struct Typelist {
  using Head = H;
  using Tail = T;
};

//==============================================================================

template <class TList>
struct Length;
template <>
struct Length<Void> {
  enum { value = 0 };
};
template <class H, class T>
struct Length<Typelist<H, T> > {
  enum { value = 1 + Length<T>::value };
};

//==============================================================================

template <class TList, unsigned int index>
struct At;
template <class H, class T>
struct At<Typelist<H, T>, 0> {
  using Result = H;
};
template <class H, class T, unsigned int i>
struct At<Typelist<H, T>, i> {
  using Result = typename At<T, i - 1>::Result;
};

//==============================================================================

template <class TList, class X>
struct IndexOf;
template <class X>
struct IndexOf<Void, X> {
  enum { value = -1 };
};
template <class X, class T>
struct IndexOf<Typelist<X, T>, X> {
  enum { value = 0 };
};
template <class H, class T, class X>
struct IndexOf<Typelist<H, T>, X> {
 private:
  enum { temp = IndexOf<T, X>::value };

 public:
  enum { value = (temp == -1 ? -1 : 1 + temp) };
};

//==============================================================================

template <class TList, class X>
struct Append;
template <>
struct Append<Void, Void> {
  using Result = Void;
};
template <class X>
struct Append<Void, X> {
  using Result = Typelist<X, Void>;
};
template <class H, class T>
struct Append<Void, Typelist<H, T> > {
  using Result = Typelist<H, T>;
};
template <class H, class T, class X>
struct Append<Typelist<H, T>, X> {
  using Result = Typelist<H, typename Append<T, X>::Result>;
};

//==============================================================================

template <class TList, class X>
struct Erase;
template <class X>
struct Erase<Void, X> {
  using Result = Void;
};
template <class X, class T>
struct Erase<Typelist<X, T>, X> {
  using Result = T;
};
template <class H, class T, class X>
struct Erase<Typelist<H, T>, X> {
  using Result = Typelist<H, typename Erase<T, X>::Result>;
};

//==============================================================================

template <class TList, class X>
struct EraseAll;
template <class X>
struct EraseAll<Void, X> {
  using Result = Void;
};
template <class X, class T>
struct EraseAll<Typelist<X, T>, X> {
  using Result = typename EraseAll<T, X>::Result;
};
template <class H, class T, class X>
struct EraseAll<Typelist<H, T>, X> {
  using Result = Typelist<H, typename EraseAll<T, X>::Result>;
};

//==============================================================================

template <class TList>
struct NoDuplicates;
template <>
struct NoDuplicates<Void> {
  using Result = Void;
};
template <class H, class T>
struct NoDuplicates<Typelist<H, T> > {
  using Result = Typelist<H, typename Erase<typename NoDuplicates<T>::Result, H>::Result>;
};

//==============================================================================

template <class TList, class X, class Y>
struct Replace;
template <class X, class Y>
struct Replace<Void, X, Y> {
  using Result = Void;
};
template <class X, class T, class Y>
struct Replace<Typelist<X, T>, X, Y> {
  using Result = Typelist<Y, T>;
};
template <class H, class T, class X, class Y>
struct Replace<Typelist<H, T>, X, Y> {
  using Result = Typelist<H, typename Replace<T, X, Y>::Result>;
};

//==============================================================================

template <class TList, class X, class Y>
struct ReplaceAll;
template <class X, class Y>
struct ReplaceAll<Void, X, Y> {
  using Result = Void;
};
template <class X, class T, class Y>
struct ReplaceAll<Typelist<X, T>, X, Y> {
  using Result = Typelist<Y, typename ReplaceAll<T, X, Y>::Result>;
};
template <class H, class T, class X, class Y>
struct ReplaceAll<Typelist<H, T>, X, Y> {
  using Result = Typelist<H, typename ReplaceAll<T, X, Y>::Result>;
};

//==============================================================================

template <class TList>
struct Reverse;
template <>
struct Reverse<Void> {
  using Result = Void;
};
template <class H, class T>
struct Reverse<Typelist<H, T> > {
  using Result = typename Append<typename Reverse<T>::Result, H>::Result;
};

//==============================================================================

template <class TList, class X>
struct MostDerived;
template <class T>
struct MostDerived<Void, T> {
  using Result = T;
};
template <class H, class Tail, class T>
struct MostDerived<Typelist<H, Tail>, T> {
 private:
  using Candidate = typename MostDerived<Tail, T>::Result;

 public:
  using Result = typename If<SuperSubclass<Candidate, H>::value, H, Candidate>::Result;
};

//==============================================================================

template <class TList>
struct DerivedToFront;
template <>
struct DerivedToFront<Void> {
  using Result = Void;
};
template <class H, class T>
struct DerivedToFront<Typelist<H, T> > {
 private:
  using TheMostDerived = typename MostDerived<T, H>::Result;

 public:
  using Result = Typelist<TheMostDerived, typename DerivedToFront<typename Replace<T, TheMostDerived, H>::Result>::Result>;
};

//==============================================================================
/// TList �̂��ꂼ��̗v�f Element �ɑ΂��APred<Status, Element>���Ăяo��.
/// �J��Ԃ����Ƃ� Status = Pred<Status, Element>::Result ������Status�Ƃ��Ďg�p�����.
template <class TList, class Status, template <class Status, class T> class Pred>
struct Fold {
  using Result = typename Fold<typename TList::Tail,                                 // �c��
                               typename Pred<Status, typename TList::Head>::Result,  // Head �̌���
                               Pred>::Result;
};
template <class Status, template <class, class> class Pred>
struct Fold<Void, Status, Pred> {
  using Result = Status;
};

//==============================================================================
/// TList �� T �����p���̉����̃N���X���܂܂�Ă���ꍇ�ATList����T����������.
template <class TList, class T>
struct RemoveIfNotMostDerived {
  using Result =
      typename If<Equals<T, typename MostDerived<TList, T>::Result>::value, TList, typename Erase<TList, T>::Result>::Result;
};

//==============================================================================
/// TList �̒��̂��ׂĂ̌^�́A��������㗬�̌^���܂܂�Ă��Ȃ��悤�ɂ���.
template <class TList>
struct MostDerivedOnly {
  using Result = typename Fold<TList, TList, RemoveIfNotMostDerived>::Result;
};

//==============================================================================
/// TList �Ɋ܂܂�邷�ׂĂ̌^�𑽏d�p�������N���X���`����.
/// GenScatterHierarchy�𗘗p�����ꍇ�͗]����vtable���ǉ�����邪�A���̃N���X�ł͍œK������Ă���.
template <class TList>
struct __declspec(novtable) Inherits : TList::Head, Inherits<typename TList::Tail> {
  using Head = typename TList::Head;
  using Tail = typename TList::Tail;
};
template <class H>
struct __declspec(novtable) Inherits<Typelist<H, Void> > : H {};
template <>
struct __declspec(novtable) Inherits<Void> {};

//==============================================================================
/// TList �� T ���܂�ł��邩�ۂ�.
template <class TList, class T>
struct Contains {
  enum { value = (IndexOf<TList, T>::value != -1) };
};

//==============================================================================
/// ���W����Ԃ�.
template <class TLhs, class TRhs>
struct Subtract {
  /// TLhs�̒�����TRhs�Ɋ܂܂��^����菜��.
  /// TRhs�ɂ͊܂܂�邪�ATLhs�Ɋ܂܂�Ȃ��^�ɂ��Ă͂Ȃɂ����Ȃ�.
  using Result = typename Fold<TRhs, TLhs, Erase>::Result;
};

//==============================================================================
/// �ϏW����Ԃ�.
template <class TLhs, class TRhs>
struct Intersection {
 private:
  using UniqueLeft = typename NoDuplicates<TLhs>::Result;

 public:
  /// TLhs��TRhs�̗����Ɋ܂܂��^�𓾂�.
  using Result = typename Subtract<UniqueLeft, typename Subtract<UniqueLeft, TRhs>::Result>::Result;
};

//==============================================================================
/// �a�W����Ԃ�.
template <class TLhs, class TRhs>
struct Union {
  /// TLhs�̖�����TRhs��A�����A�d������菜��.
  using Result = typename NoDuplicates<typename Fold<TLhs, TRhs, Append>::Result>::Result;
};

//�r���I�a�W��
// Disjunction
}  // namespace meta
}  // namespace mew
