#include "serializetools.h"

using namespace across::config;

SerializeTools::SerializeTools() {}

std::optional<SIP008>
SerializeTools::sip008Parser(const std::string& data)
{
  JsonTools json_tools;
  if (!json_tools.setData(data)) {
    return {};
  }

  auto root = json_tools.getRoot();
  SIP008 sip008;

  try {
    if (root.is_null() || root.empty() || !root.contains("version")) {
      return {};
    }
  } catch (Json::exception e) {
    return {};
  }

  sip008.version = root["version"];

  if (root.contains("username"))
    sip008.username = root["username"];

  if (root.contains("id"))
    sip008.user_uuid = root["id"];

  if (root.contains("bytes_used"))
    sip008.bytes_used = root["bytes_used"];

  if (root.contains("bytes_reminging"))
    sip008.bytes_remaining = root["bytes_reminging"];

  for (auto& item : root["servers"]) {
    SIP008::Server server;

    if (item.contains("id"))
      server.id = item["id"];

    if (item.contains("remarks"))
      server.remarks = item["remarks"];

    if (item.contains("server") && item.contains("server_port") &&
        item.contains("password") && item.contains("method")) {
      server.server = item["server"];
      server.server_port = item["server_port"];
      server.password = item["password"];
      server.method = item["method"];
    } else {
      continue;
    }

    if (item.contains("plugin")) {
      server.plugin = item["plugin"];

      if (item.contains("plugin_opts"))
        server.plugin_opts = item["plugin_opts"];
    }

    sip008.servers.emplace_back(server);
  }

  return sip008;
}

bool
SerializeTools::decodeOutboundFromURL(NodeInfo& node, const QString& raw_url)
{
  bool result = false;
  node.url = raw_url;

  do {
    QUrl url(raw_url);
    auto scheme = magic_enum::enum_cast<SchemeType>(url.scheme().toStdString());

    if (!scheme.has_value()) {
      break;
    }

    switch (scheme.value()) {
      case SchemeType::ss:
        result = SerializeTools::setShadowsocksOutboundFromURL(node, url);
        break;
      case SchemeType::vmess:
        result = SerializeTools::setVMessOutboundFromBase64(
          node, raw_url.toStdString());
        break;
      case SchemeType::trojan:
        result = SerializeTools::setTrojanOutboundFromURL(node, url);
        break;
      default:
        break;
    }
  } while (false);

  return result;
}

bool
SerializeTools::setShadowsocksOutboundFromURL(NodeInfo& node, const QUrl& url)
{
  bool result = false;

  do {
    auto sip008_server = SerializeTools::sip002Decode(url);
    if (!sip008_server.has_value()) {
      break;
    }

    ShadowsocksObject::OutboundSettingObject outbound_setting;
    outbound_setting.fromSIP008Server(sip008_server.value());

    OutboundObject outbound_object;
    outbound_object.appendShadowsocksObject(outbound_setting);

    node.protocol = across::EntryType::shadowsocks;
    node.name = QString::fromStdString(sip008_server->remarks);
    node.address = QString::fromStdString(outbound_setting.address);
    node.port = outbound_setting.port;
    node.password = QString::fromStdString(outbound_setting.password);
    node.raw = QString::fromStdString(outbound_object.toObject().dump());

    result = true;
  } while (false);

  return result;
}

bool
SerializeTools::setVMessOutboundFromBase64(NodeInfo& node,
                                           const std::string& data)
{
  bool result = false;

  do {
    auto vmess_meta = SerializeTools::vmessBase64Decode(data);
    if (!vmess_meta.has_value()) {
      break;
    }

    auto outbound_object = vmess_meta->outbound_object;

    node.protocol = across::EntryType::vmess;
    node.name = QString::fromStdString(vmess_meta->name);
    node.address = QString::fromStdString(vmess_meta->address);
    node.port = vmess_meta->port;
    node.password = QString::fromStdString(vmess_meta->password);
    node.raw = QString::fromStdString(outbound_object.toObject().dump());

    result = true;
  } while (false);

  return result;
}

bool
SerializeTools::setTrojanOutboundFromURL(NodeInfo& node, const QUrl& url)
{
  bool result = false;

  do {
    auto trojan_meta = SerializeTools::trojanDecode(url);
    if (!trojan_meta.has_value()) {
      break;
    }
    auto outbound_object = trojan_meta->outbound_object;

    node.protocol = across::EntryType::trojan;
    node.name = QString::fromStdString(trojan_meta->name);
    node.address = QString::fromStdString(trojan_meta->address);
    node.port = trojan_meta->port;
    node.password = QString::fromStdString(trojan_meta->password);
    node.raw = QString::fromStdString(outbound_object.toObject().dump());

    result = true;
  } while (false);

  return result;
}

std::optional<SIP008::Server>
SerializeTools::sip002Decode(const QUrl& url)
{
  // url scheme:
  // ss://<websafe-base64-encode-utf8(method:password)>@hostname:port/?plugin"#"tag

  SIP008::Server server;

  QString user_info = QByteArray::fromBase64(url.userInfo().toUtf8());

  server.method = user_info.split(":").first().toStdString();
  server.password = user_info.split(":").last().toStdString();
  server.server = url.host().toStdString();
  server.server_port = url.port();
  server.remarks = url.fragment(QUrl::FullyDecoded).toStdString();

  if (server.method.empty() || server.password.empty() ||
      server.server.empty()) {
    return {};
  }

  return server;
}

std::optional<QUrl>
SerializeTools::sip002Encode(const SIP008::Server& node)
{
  // url scheme:
  // ss://<websafe-base64-encode-utf8(method:password)>@hostname:port/?plugin"#"tag

  QUrl url;
  QString user_info = QString("%1:%2")
                        .arg(QString::fromStdString(node.method),
                             QString::fromStdString(node.password))
                        .toUtf8()
                        .toBase64(QByteArray::Base64UrlEncoding);

  url.setScheme("ss");
  url.setHost(QString::fromStdString(node.server));
  url.setPort(node.server_port);
  url.setUserInfo(user_info);
  url.setFragment(QString::fromStdString(node.remarks));

  return url;
}

std::optional<URLMetaObject>
SerializeTools::trojanDecode(const QUrl& url)
{
  // url scheme:
  // trojan://<password>@<host>:<port>?sni=<server_name>&allowinsecure=<allow_insecure>&alpn=h2%0Ahttp/1.1#<name>

  do {
    OutboundObject outbound_object;

    TrojanObject::OutboundSettingObject outbound_setting;
    outbound_setting.address = url.host().toStdString();
    outbound_setting.port = url.port();
    outbound_setting.password = url.userInfo().toStdString();

    outbound_object.appendTrojanObject(outbound_setting);

    // query need to be verified
    if (url.hasQuery()) {
      QUrlQuery url_query(url);
      TransportObject::OutboundStreamObject outbound_stream;
      TransportObject::TLSObject tls;

      if (url_query.hasQueryItem("sni")) {
        tls.server_name = url_query.queryItemValue("sni").toStdString();
      }

      if (url_query.hasQueryItem("allowinsecure")) {
        tls.allow_insecure =
          url_query.queryItemValue("allowinsecure") == "true";
      }

      if (url_query.hasQueryItem("alpn")) {
        auto alpn = url_query.queryItemValue("alpn", QUrl::FullyDecoded);

        QStringList values;

        if (alpn.contains(",")) {
          values = alpn.split(",");
        } else if (alpn.contains("\n")) {
          values = alpn.split("\n");
        }

        if (values.empty()) {
          tls.appendAlpn(alpn.toStdString());
        } else {
          for (auto& item : values) {
            tls.appendAlpn(item.toStdString());
          }
        }
      }

      outbound_stream.setTLSSetting(tls);
      outbound_object.setTransportStreamObject(outbound_stream);
    }

    return URLMetaObject{ .name =
                            url.fragment(QUrl::FullyDecoded).toStdString(),
                          .address = outbound_setting.address,
                          .port = outbound_setting.port,
                          .password = outbound_setting.password,
                          .outbound_object = outbound_object };
  } while (false);

  return {};
}

std::optional<QUrl>
SerializeTools::trojanEncode(const URLMetaObject& meta)
{
  // url scheme:
  // trojan://<password>@<host>:<port>?sni=<server_name>&allowinsecure=<allow_insecure>&alpn=h2%0Ahttp/1.1#<name>

  QUrl url;
  QUrlQuery query;

  url.setScheme("trojan");
  url.setHost(QString::fromStdString(meta.address));
  url.setPort(meta.port);
  url.setUserInfo(QString::fromStdString(meta.password));
  url.setFragment(QString::fromStdString(meta.name));

  if (auto stream = meta.outbound_object.stream_settings; !stream.is_null()) {
    if (auto tls = stream["tlsSettings"]; !tls.is_null()) {
      if (auto sni = tls["serverName"]; !sni.is_null() && sni.is_string()) {
        query.addQueryItem("sni", QString::fromStdString(sni));
      }

      if (auto allow_insecure = tls["allowInsecure"];
          !allow_insecure.is_null() && allow_insecure.is_boolean()) {
        if (allow_insecure.get<bool>()) {
          query.addQueryItem("allowinsecure", "true");
        } else {
          query.addQueryItem("allowinsecure", "false");
        }
      }

      if (auto alpn = tls["alpn"]; !alpn.is_null() && alpn.is_array()) {
        QString alpn_string;
        auto alpn_size = alpn.size();

        for (auto& alpn_item : alpn) {
          alpn_string.append(
            QString::fromStdString(alpn_item.get<std::string>()));

          if (alpn_size > 1) {
            alpn_string.append("%0A");
            alpn_size--;
          }
        }

        query.addQueryItem("alpn", alpn_string);
      }
    }
  }

  url.setQuery(query);

  return url;
}

std::optional<URLMetaObject>
SerializeTools::vmessBase64Decode(const std::string& data)
{
  do {
    QString info = QString::fromStdString(data).split("://").takeLast();
    if (info.isEmpty()) {
      break;
    }

    QString base64_str = QByteArray::fromBase64(info.toUtf8());

    JsonTools json_tools;
    if (!json_tools.setData(base64_str.toStdString())) {
      break;
    }
    auto root = json_tools.getRoot();

    VMessObject::UserObject user_object;
    user_object.id = root["id"];
    user_object.alter_id = root["aid"];

    VMessObject::OutboundSettingObject outbound_setting;
    outbound_setting.address = root["add"];
    outbound_setting.port = root["port"];
    outbound_setting.appendUserObject(user_object);

    TransportObject::OutboundStreamObject outbound_stream;
    auto network = magic_enum::enum_cast<TransportObject::Network>(
      root["net"].get<std::string>());
    if (!network.has_value()) {
      break;
    } else {
      outbound_stream.network = network.value();
    }

    TransportObject::WebSocketObject websocket;

    switch (outbound_stream.network) {
      case TransportObject::Network::ws:
        websocket.setHost(root["host"]);
        websocket.path = root["path"];
        outbound_stream.setWebsocketSetting(websocket);
      default:
        break;
    }

    if (!root["tls"].is_null() || !root["tls"].empty()) {
      outbound_stream.security = root["tls"].get<std::string>();
    }

    if (!root["sni"].is_null() || !root["sni"].empty()) {
      TransportObject::TLSObject tls;
      tls.server_name = root["sni"].get<std::string>();
      outbound_stream.setTLSSetting(tls);
    }

    OutboundObject outbound_object;
    outbound_object.appendVMessObject(outbound_setting);
    outbound_object.setTransportStreamObject(outbound_stream);

    return URLMetaObject{ root["ps"].get<std::string>(),
                          outbound_setting.address,
                          outbound_setting.port,
                          user_object.id,
                          outbound_object };
  } while (false);

  return {};
}

std::optional<QUrl>
SerializeTools::vmessBase64Encode(const URLMetaObject& meta)
{
  // url scheme:
  // vmess://<base64EncodeJson>
  // {
  //     "v": "2",
  //     "ps": "Names",
  //     "add": "111.111.111.111",
  //     "port": "32000",
  //     "id": "1386f85e-657b-4d6e-9d56-78badb75e1fd",
  //     "aid": "100",
  //     "scy": "zero",
  //     "net": "tcp",
  //     "type": "none",
  //     "host": "www.bbb.com",
  //     "path": "/",
  //     "tls": "tls",
  //     "sni": "www.ccc.com"
  //  }

  QUrl url;
  url.setScheme("vmess");

  Json root;
  root["v"] = "2";
  root["ps"] = meta.name;
  root["add"] = meta.address;
  root["port"] = meta.port;
  root["id"] = meta.password;
  root["sni"] = meta.address;

  if (auto vnext = meta.outbound_object.servers; !vnext.empty()) {
    if (auto user_object = vnext[0]; !user_object.is_null()) {
      if (auto aid = user_object["aid"]; !aid.is_null()) {
        root["aid"] = aid;
      } else {
        root["aid"] = 0;
      }

      if (auto scy = user_object["security"]; !scy.is_null()) {
        root["scy"] = scy;
      } else {
        root["scy"] = "auto";
      }
    }
  }

  if (auto stream = meta.outbound_object.stream_settings; !stream.is_null()) {
    if (auto network = stream["network"];
        !network.is_null() && network.is_string()) {
      root["net"] = network.get<std::string>();

      if (network.get<std::string>() == "ws") {
        if (auto ws_setting = stream["wsSetting"]; !ws_setting.is_null()) {
          if (auto host = ws_setting["headers"]["Host"]; !host.is_null()) {
            root["host"] = host;
          }
          if (auto path = ws_setting["path"]; !path.is_null()) {
            root["path"] = path;
          }
        }
      } else {
        root["host"] = "";
        root["path"] = "";
      }
    }

    if (auto security = stream["security"]; !security.is_null()) {
      root["tls"] = "tls";
    } else {
      root["tls"] = "none";
    }
  }

  // TODO: support fake stream type
  root["type"] = "none";

  url.setUserInfo(
    QString::fromStdString(root.get<std::string>()).toUtf8().toBase64());

  return url;
}
