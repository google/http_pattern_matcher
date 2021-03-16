// Copyright 2021 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////
//
#include "include/http_pattern_matcher/path_matcher.h"

#include "include/http_pattern_matcher/http_template.h"

namespace google {
namespace http {
namespace pattern_matcher {

namespace {

inline bool IsReservedChar(char c) {
  // Reserved characters according to RFC 6570
  switch (c) {
    case '!':
    case '#':
    case '$':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case ',':
    case '/':
    case ':':
    case ';':
    case '=':
    case '?':
    case '@':
    case '[':
    case ']':
      return true;
    default:
      return false;
  }
}

// Check if an ASCII character is a hex digit.  We can't use ctype's
// isxdigit() because it is affected by locale. This function is applied
// to the escaped characters in a url, not to natural-language
// strings, so locale should not be taken into account.
inline bool ascii_isxdigit(char c) {
  return ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F') ||
         ('0' <= c && c <= '9');
}

inline int hex_digit_to_int(char c) {
  /* Assume ASCII. */
  int x = static_cast<unsigned char>(c);
  if (x > '9') {
    x += 9;
  }
  return x & 0xf;
}

}  // namespace

bool GetEscapedChar(const std::string& src, size_t i,
                    UrlUnescapeSpec unescape_spec, char* out) {
  if (i + 2 < src.size() && src[i] == '%') {
    if (ascii_isxdigit(src[i + 1]) && ascii_isxdigit(src[i + 2])) {
      char c =
          (hex_digit_to_int(src[i + 1]) << 4) | hex_digit_to_int(src[i + 2]);
      switch (unescape_spec) {
        case UrlUnescapeSpec::kAllCharactersExceptReserved:
          if (IsReservedChar(c)) {
            return false;
          }
          break;
        case UrlUnescapeSpec::kAllCharactersExceptSlash:
          if (c == '/') {
            return false;
          }
          break;
        case UrlUnescapeSpec::kAllCharacters:
          break;
      }
      *out = c;
      return true;
    }
  }
  return false;
}

// Unescapes string 'part' and returns the unescaped string. Reserved characters
// (as specified in RFC 6570) are not escaped if unescape_reserved_chars is
// false.
std::string UrlUnescapeString(const std::string& part,
                              UrlUnescapeSpec unescape_spec) {
  std::string unescaped;
  // Check whether we need to escape at all.
  bool needs_unescaping = false;
  char ch = '\0';
  for (size_t i = 0; i < part.size(); ++i) {
    if (GetEscapedChar(part, i, unescape_spec, &ch)) {
      needs_unescaping = true;
      break;
    }
  }
  if (!needs_unescaping) {
    unescaped = part;
    return unescaped;
  }

  unescaped.resize(part.size());

  char* begin = &(unescaped)[0];
  char* p = begin;

  for (size_t i = 0; i < part.size();) {
    if (GetEscapedChar(part, i, unescape_spec, &ch)) {
      *p++ = ch;
      i += 3;
    } else {
      *p++ = part[i];
      i += 1;
    }
  }

  unescaped.resize(p - begin);
  return unescaped;
}

// Converts a request path into a format that can be used to perform a request
// lookup in the PathMatcher trie. This utility method sanitizes the request
// path and then splits the path into slash separated parts. Returns an empty
// vector if the sanitized path is "/".
//
// custom_verbs is a set of configured custom verbs that are used to match
// against any custom verbs in request path. If the request_path contains a
// custom verb not found in custom_verbs, it is treated as a part of the path.
//
// - Strips off query string: "/a?foo=bar" --> "/a"
// - Collapses extra slashes: "///" --> "/"
std::vector<std::string> ExtractRequestParts(
    std::string path, const std::unordered_set<std::string>& custom_verbs,
    std::string& verb) {
  // Remove query parameters.
  path = path.substr(0, path.find_first_of('?'));

  // Replace last ':' with '/' to handle custom verb.
  // But not for /foo:bar/const.
  std::size_t last_colon_pos = path.find_last_of(':');
  std::size_t last_slash_pos = path.find_last_of('/');
  if (last_colon_pos != std::string::npos && last_colon_pos > last_slash_pos) {
    std::string tmp_verb = path.substr(last_colon_pos + 1);
    // only verb in the configured custom verbs, treat it as verb
    if (custom_verbs.find(tmp_verb) != custom_verbs.end()) {
      verb = tmp_verb;
      path = path.substr(0, last_colon_pos);
    }
  }

  std::vector<std::string> result;
  if (path.size() > 0) {
    result = absl::StrSplit(path.substr(1), '/');
  }
  // Removes all trailing empty parts caused by extra "/".
  while (!result.empty() && (*(--result.end())).empty()) {
    result.pop_back();
  }
  return result;
}

// Looks up on a PathMatcherNode.
PathMatcherLookupResult LookupInPathMatcherNode(
    const PathMatcherNode& root, const std::vector<std::string>& parts,
    const HttpMethod& http_method) {
  PathMatcherLookupResult result;
  root.LookupPath(parts.begin(), parts.end(), http_method, &result);
  return result;
}

PathMatcherNode::PathInfo TransformHttpTemplate(const HttpTemplate& ht) {
  PathMatcherNode::PathInfo::Builder builder;

  for (const std::string& part : ht.segments()) {
    builder.AppendLiteralNode(part);
  }

  return builder.Build();
}

}  // namespace pattern_matcher
}  // namespace http
}  // namespace google
