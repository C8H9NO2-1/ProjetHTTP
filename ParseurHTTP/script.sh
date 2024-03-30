#!bin/bash

recherche() {
    echo -e "Recherche de $rulename\n..."
    ./main "$rulename"

    echo -e "\n====================================================================================================\n"
}

echo -e "====================================================================================================\n"

rulename="HTTP-message"
recherche

rulename="start-line"
recherche

rulename="method"
recherche

rulename="tchar"
#recherche

rulename="SP"
#recherche

rulename="request-target"
recherche

rulename="absolute-path"
recherche

rulename="case_insensitive_string"
#recherche

rulename="segment"
recherche

rulename="pchar"
recherche

rulename="query"
recherche

rulename="HTTP-version"
recherche

rulename="HTTP-name"
recherche

rulename="DIGIT"
#recherche

rulename="CRLF"
#recherche

rulename="header-field"
recherche

rulename="Connection-header"
recherche

rulename="OWS"
#recherche

rulename="Connection"
recherche

rulename="connection-option"
recherche

rulename="Cookie-header"
recherche

rulename="cookie-string"
recherche

rulename="cookie-pair"
recherche

rulename="cookie-name"
recherche

rulename="cookie-value"
recherche

rulename="DQUOTE"
#recherche

rulename="cookie-octet"
#recherche

rulename="Transfer-Encoding-header"
recherche

rulename="Transfer-Encoding"
recherche

rulename="transfer-coding"
recherche

rulename="transfer-extension"
recherche

rulename="transfer-parameter"
recherche

rulename="quoted-string"
recherche

rulename="qdtext"
#recherche

rulename="quoted-pair"
#recherche

rulename="HTAB"
#recherche

rulename="VCHAR"
#recherche

rulename="obs-text"
#recherche

rulename="Expect-header"
recherche

rulename="Expect"
recherche

rulename="Host-header"
recherche

rulename="Host"
recherche

rulename="host"
recherche

rulename="IP-literal"
recherche

rulename="IPv6address"
recherche

rulename="h16"
recherche

rulename="IPvFuture"
recherche

rulename="HEXDIG"
#recherche

rulename="unreserved"
recherche

rulename="sub-delims"
recherche

rulename="IPv4address"
recherche

rulename="dec-octet"
#recherche

rulename="reg-name"
recherche

rulename="pct-encoded"
recherche

rulename="port"
recherche

rulename="Content-Length-header"
recherche

rulename="Content-Length"
recherche

rulename="Content-Type-header"
recherche

rulename="media-type"
recherche

rulename="type"
recherche

rulename="subtype"
recherche

rulename="parameter"
recherche

rulename="field-name"
recherche

rulename="field-value"
recherche

rulename="field-content"
recherche

rulename="field-vchar"
recherche

rulename="obs-fold"
recherche

rulename="message-body"
recherche

rulename="OCTET"
recherche

rulename="token"
recherche

