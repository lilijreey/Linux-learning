/**
 * @file     unique_ptr.cpp
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/08/2013 03:05:48 PM
 *
 */

/// unique_ptr 使用RAII 来管理自动管理
//ownership 不需要恭喜
// * 同一时间只有一个unique_ptr 可以持有一个原始指针
// * 在 unique_ptr destroy的时候会 delete 管理的原始指针，
// * unique_ptr 的move语义，可以让unique_ptr 管理的对象在不同unique_ptr 
//          对象中转换.
//        e.g. std::unique_ptr<XX> a = new XX;
//             std::unique_ptr<XX> b(std::move(a));
//             //a 现在为空， a.get() == nullpter
//              
//              
//

/// shared_ptr 使用RAII 来管理自动管理
//ownership 共享
///std::make_shared_ptr
