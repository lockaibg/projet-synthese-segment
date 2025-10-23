#include "rational.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <geometry.h>

#include "util.h"

long long get_numerator(struct Rational r) {
	return r.numerator;
}

long long get_denominator(struct Rational r) {
	return r.denominator;
}

void set_numerator(struct Rational * r, long long new_numerator) {
	assert(r);
	r->numerator = new_numerator;
}

void set_denominator(struct Rational * r, long long new_denominator) {
	assert(r);
	r->denominator = new_denominator;
}

/**
 * Restitue le Plus Grand Diviseur Commun de \p a et \p b .
 *
 * @param[in] a
 * @param[in] b
 * @return PGCD(a,b)
 */
static long long gcd(long long a, long long b) {
	while (b != 0) {
		int temp = b;
		b = a % b;  // Calcul du reste
		a = temp;   // Mise à jour de a
	}
	return a;  // Le PGCD est le dernier reste non nul

}

void simplify(struct Rational * r) {
	assert(r);
	//check dénominateur pas zéro
	if (r->denominator == 0) {
		printf("erreur, zero au denominateur\n");
		exit(0);
	}
	if (gcd(r->numerator, r->denominator) != 1) {
		long long gcda = gcd(r->numerator, r->denominator);
		set_denominator(r, r->denominator/gcda);
		set_numerator(r, r->numerator/gcda);
	}
	if (r->denominator < 0) {
		set_denominator(r, -(r->denominator));
		set_numerator(r, -(r->numerator));
	}
}

int gt(struct Rational a, struct Rational b) {
  	simplify(&a);
	simplify(&b);
	if (a.numerator*b.denominator > b.numerator*a.denominator)
		return 1;
	else
		return 0;
}

int lt(struct Rational a, struct Rational b) {
	if(a.denominator != b.denominator) {
		struct Rational r;
		set_denominator(&r,(get_denominator(a)*get_denominator(b)));
		set_numerator(&r,get_denominator(a)*get_numerator(b));
		struct Rational r2;
		set_denominator(&r2,(get_denominator(b)*get_denominator(a)));
		set_numerator(&r2,get_denominator(b)*get_numerator(a));
	}
	if(a.numerator < b.numerator)
		return 1;
	else
		return 0;
}

int gte(struct Rational a, struct Rational b) {
	if(a.denominator != b.denominator) {
		struct Rational r;
		set_denominator(&r,(get_denominator(a)*get_denominator(b)));
		set_numerator(&r,get_denominator(a)*get_numerator(b));
		struct Rational r2;
		set_denominator(&r2,(get_denominator(b)*get_denominator(a)));
		set_numerator(&r2,get_denominator(b)*get_numerator(a));
	}
	if(a.numerator >= b.numerator)
		return 1;
	else
		return 0;
}

int lte(struct Rational a, struct Rational b) {
	if(a.denominator != b.denominator) {
		struct Rational r;
		set_denominator(&r,(get_denominator(a)*get_denominator(b)));
		set_numerator(&r,get_denominator(a)*get_numerator(b));
		struct Rational r2;
		set_denominator(&r2,(get_denominator(b)*get_denominator(a)));
		set_numerator(&r2,get_denominator(b)*get_numerator(a));
	}
	if(a.numerator <= b.numerator)
		return 1;
	else
		return 0;
}

int eq(struct Rational a, struct Rational b) {
  	simplify(&a);
	simplify(&b);
	if(a.numerator == b.numerator && a.denominator == b.denominator)
		return 1;
	else
		return 0;
}

int neq(struct Rational a, struct Rational b) {
	simplify(&a);
	simplify(&b);
	if(a.numerator != b.numerator || a.denominator != b.denominator)
		return 1;
	else
		return 0;
}

// Pensez à simplifier le résultat.
struct Rational radd(struct Rational a, struct Rational b) {
		struct Rational result;
		result.numerator = a.numerator * b.denominator + b.numerator * a.denominator;
	result.denominator = a.denominator * b.denominator;
	simplify(&result);
	return result;
	}

// Pensez à simplifier le résultat.
struct Rational rsub(struct Rational a, struct Rational b) {
	struct Rational result;
	result.numerator= a.numerator * b.denominator -a.denominator * b.numerator;
	result.denominator= a.denominator * b.denominator;
	simplify(&result);
	return result;
}

struct Rational valeur_abs(struct Rational a) {
	struct Rational result;
	if(a.numerator > 0){
        if(a.denominator > 0){
		    result.numerator = a.numerator;
		    result.denominator = a.denominator;
		    } else {
		      	result.numerator = a.numerator;
				result.denominator = -a.denominator;
		    }
	} else if(a.denominator > 0){
	    result.numerator = -a.numerator;
	    result.denominator = -a.denominator;
	}else{
		result.numerator = a.numerator;
		result.denominator = a.denominator;
	}
    return result;
}

// Pensez à simplifier le résultat.
struct Rational rmul(struct Rational a, struct Rational b) {
	struct Rational result;
	result.numerator = a.numerator * b.numerator;
	result.denominator= a.denominator * b.denominator;
	simplify(&result);
	return result;
}

// Pensez à simplifier le résultat.
struct Rational rdiv(struct Rational a, struct Rational b) {

	struct Rational result;
	if ((b.numerator == 0) || (a.denominator == 0)) {
		ShowMessage("Dénominateur nul",1);
	}
	result.numerator = a.numerator * b.denominator;
	result.denominator= a.denominator * b.numerator;
	simplify(&result);
	return result;
}

struct Rational rmax(struct Rational a, struct Rational b) {
	if (gt(a,b))
		return a;
	else
		return b;
}

struct Rational rmin(struct Rational a, struct Rational b) {
	if (lt(a,b))
		return a;
	else
		return b;
}

void print_rational(struct Rational r) {
	printf("%lld/%lld", r.numerator, r.denominator);
}