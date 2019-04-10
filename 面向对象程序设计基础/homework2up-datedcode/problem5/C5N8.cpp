#include <bits/stdc++.h>
#include "C5N8.h"

#define PR(a) std::cout << "Type name: " << #a << "    \tAddress: " << (int*)&a << std::endl;
/**
 * [C5N8::showMap description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * show all vars's address in the memory during the process
 */
void C5N8::showMap()
{
	PR(private_i32);
	PR(private_i64);
	PR(private_i8);
	PR(private_f32);
	PR(private_f64);
	PR(protected_i32);
	PR(protected_i64);
	PR(protected_i8);
	PR(protected_f32);
	PR(protected_f64);
	PR(public_i32);
	PR(public_i64);
	PR(public_i8);
	PR(public_f32);
	PR(public_f64);
}
