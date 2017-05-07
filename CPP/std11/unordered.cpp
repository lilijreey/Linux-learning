/*
 *  Description: 
 */
#include <stdio.h>
#include <unordered_set>
#include <set>
#include <sys/time.h>

// EE unorder 总结
// 1. unorder的默认最大负载因子为1.0 
// 2. 如果负载因子大于最大值，容器会扩充新的bucket.
// 3. 默认的设置几乎不会产生冲突。  unorder* 容器的效率比非hash的高很多
//
// Qus. 新增bucket时会copy原来的数据吗，也就是会重新hash所有数据吗？

// 基本操作
// EE lookup
//   find, count, equal_range

// EE bucket interface
// +. bucket_count() 返回容器当前的桶数
// +. max_bucket_count() 返回系统允许的容器最大能够有的bucket数量，
//                    显示结果和max_size 相同
// +. bucket_size(bucket_index)  返回一个指定bucket中容纳的元素数量
// +. bucket(const Key &key)     返回一个指定key所在的bucket的index
// +. begin/cbegin end/cend      返回bucket迭代器

// EE Hash 因子 factor
// +. load_factor()  return 每个map的负载因子
// +. max_load_factor() const return 容器可以允许的最大负载因子
// +. max_load_factor(float newFactor)  设置新的最大负载因子
//                        如果超过最大负载因子容器会自动扩大bucket
// +. rehash(size_type count);
//             设置容器至少有count个bucket。 容器会自动把bucket设置为
//             大于count的素数。 这个操作会重新构建hashTable
//             如果count的现在的size小，重新生成的bueckt最小是size()/max_load_factor()
//             也就是不会删除已有的元素
// + reserve(size_type count); 
//            设置bucket 的数量, 容器会自动设置为大于count的素数
//            更有效的方式是， rehash(std::ceil(count / max_load_factor());

// {{{ 输出结果
//      sizeof:56
//      size:4
//      max_size:1152921504606846975
//      bucket_count:5
//      max_bucket_count:1152921504606846975
//      bucket 1 member count:1
//      key 3 in bucket :3
//      laod factor is 0.800000
//      max_load_factor is 1.000000
//      ----------------1W--------------------------------
//      sizeof:56
//      size:10000
//      max_size:1152921504606846975
//      bucket_count:15173
//      max_bucket_count:1152921504606846975
//      bucket 1 member count:1
//      key 3 in bucket :3
//      laod factor is 0.659065
//      max_load_factor is 1.000000
//      ------------------100W--------------------------------
//      sizeof:56
//      size:10000000
//      max_size:1152921504606846975
//      bucket_count:17961079
//      max_bucket_count:1152921504606846975
//      bucket 1 member count:1
//      key 3 in bucket :3
//      laod factor is 0.556759
//      max_load_factor is 1.000000
//
// }}}
// {{{ main
#if 0
int main() {
    //下面的sizeof都是36
    std::unordered_set<int> ss = {3,4,5,6};
    printf ( "sizeof:%lu\n", sizeof(ss) );
    printf ( "size:%lu\n", ss.size() );
    printf ( "max_size:%lu\n", ss.max_size() );

    printf ( "bucket_count:%lu\n", ss.bucket_count());
    printf ( "max_bucket_count:%lu\n", ss.max_bucket_count());
    printf ( "bucket 1 member count:%lu\n", ss.bucket_size(1));
    printf ( "key 3 in bucket :%lu\n", ss.bucket(3));
    printf ( "laod factor is %f\n", ss.load_factor());
    printf ( "max_load_factor is %f\n", ss.max_load_factor());

    printf("----------------1W--------------------------------\n");

    std::unordered_set<int> s1;
    s1.reserve(1000000);
    for (int i=0; i != 10000; ++i)
	    s1.insert(i);
    printf ( "sizeof:%lu\n", sizeof(s1) );
    printf ( "size:%lu\n", s1.size() );
    printf ( "max_size:%lu\n", s1.max_size() );

    //bucket interface
    printf ( "bucket_count:%lu\n", s1.bucket_count());
    printf ( "max_bucket_count:%lu\n", s1.max_bucket_count());
    printf ( "bucket 1 member count:%lu\n", s1.bucket_size(1));
    printf ( "key 3 in bucket :%lu\n", s1.bucket(3));
    printf ( "laod factor is %f\n", s1.load_factor());
    printf ( "max_load_factor is %f\n", s1.max_load_factor());

    printf("------------------100W--------------------------------\n");
    std::unordered_set<int> s2;
    for (int i=0; i != 10000000; ++i)
	    s2.insert(i);
    printf ( "sizeof:%lu\n", sizeof(s2) );
    printf ( "size:%lu\n", s2.size() );
    printf ( "max_size:%lu\n", s2.max_size() );

    //bucket interface
    printf ( "bucket_count:%lu\n", s2.bucket_count());
    printf ( "max_bucket_count:%lu\n", s2.max_bucket_count());
    printf ( "bucket 1 member count:%lu\n", s2.bucket_size(1));
    printf ( "key 3 in bucket :%lu\n", s2.bucket(3));
    printf ( "laod factor is %f\n", s2.load_factor());
    printf ( "max_load_factor is %f\n", s2.max_load_factor());
    return 0 ;
}
#endif
// }}}


// {{{ unorder 效率
#if 1
int main() 
{
  // 1. 现rehash 在插入和 不rehash插入 比较;
  // 终结： hash 和 rehash的效率差别不大
  // 2. hash 比map的insert高5倍左右
  const int size=100000;
  struct timeval res;
  struct timeval res1;


  std::set<int> ns;
  gettimeofday(&res, NULL);
  for (int i=0; i != size; ++i)
    ns.insert(i);
  gettimeofday(&res1, NULL);
  printf("not: sec:%lu, usec:%lu\n", 
         res1.tv_sec - res.tv_sec, 
         res1.tv_usec - res.tv_usec);

  std::unordered_set<int> rs;
  rs.rehash(size);
  gettimeofday(&res, NULL);
  for (int i=0; i != size; ++i)
    rs.insert(i);
  gettimeofday(&res1, NULL);
  printf("rehash: sec:%lu, usec:%lu\n", 
         res1.tv_sec - res.tv_sec, 
         res1.tv_usec - res.tv_usec);
}
#endif 

//
// }}}

//set vim:foldmethod=marker
//
