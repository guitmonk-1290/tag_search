#include <iostream>
#include <string>
#include <vector>
#include <stack>

#define MAX_DEPTH 3
#define MIN_DEPTH 1

typedef long long int ll;

// global containers
std::vector<std::string> divs;
std::vector<ll> k_div_pos;
std::stack<char> stk;
std::vector<ll> cls_pos;

// get ID of a div tag
std::string getID(std::string _html, ll div_pos) {

	std::string ID = "";
	std::string search_key = "id";

	while (_html[div_pos] != '>') {
		char a = _html[div_pos];
		char b = _html[div_pos + 1];
		std::string temp = std::string(1, a) + b;
		if (temp == search_key) {
			div_pos += 4;
			while (_html[div_pos] != '"') {
				ID += _html[div_pos];
				div_pos++;
			}
			return ID;
		}

		div_pos++;
	}

	return "";
}

// get class of a div tag
std::string getClass(std::string _html, ll div_pos) {

	// Are there divs with no ID and Class present??
	std::string _class = "";
	std::string Class = "";

	ll i;
	for (i = div_pos + 2; i < div_pos + 7; i++) _class += _html[i];
	if (_class == "class") {
		div_pos = i + 2;
		while (_html[div_pos] != '"') {
			Class += _html[div_pos];
			div_pos++;
		}
		return Class;
	}
	else {
		// find until 'class' keyword is found -> 2 pointers approach
		ll i = div_pos + 2;
		ll j = div_pos + 6;

		while (_html[j] != '>') {
			i++; j++;
			// _class[0] = _html[i];
			// _class[4] = _html[j];
			_class += _html[j];
			_class.erase(0, 1);
			if (_class == "class") {
				i += 7;
				while (_html[i] != '"') {
					Class += _html[i];
					i++;
				}
				return Class;
			}
		}
	}

	return "";
}

std::vector<ll> parse(std::string _html, std::string search_tag, std::string search_keyword = "") {
	k_div_pos.clear();

	bool tag_found = false;
	bool keyword_found = false;
	int stk_i = 0;
    bool _script = false;

	ll n = _html.length();
	// tag data string
	std::string tag_str = "";
	// keyword data string
	std::string keyword_str = "";

	for (ll i = 0; i < n; i++)
	{
		char ch = _html[i];
		if (ch == '<')
		{
			stk.push('<');
			stk_i++;
			keyword_str = "";
		}
		else if (ch == '>')
		{
			tag_str = "";
			stk.pop();
		}
		else {
			// check tag if stack has '<'
			if (stk.empty() == false) {
				tag_str += ch;
                if (tag_str == "script" || tag_str == "/script") {
                    // ignore all chars until '</script> is found'
                    _script = !_script;
                }
				if (tag_str == search_tag) {
					cls_pos.push_back(i);
				}
			}
			else {
                if(!_script) {
                    // default behaviour -> look for tags only
                    if (search_keyword == "") continue;
                    keyword_str += ch;
                    if (keyword_str == search_keyword) {
                        ll d_pos = cls_pos.back();
                        k_div_pos.push_back(d_pos);
                    }
                }
			}
		}
	}

	return cls_pos;
}
