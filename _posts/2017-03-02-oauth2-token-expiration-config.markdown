---
layout: post
author: gelnyang
comments: true
date: 2017-03-02
title: Oauth2 token expiration config
categories:
- experience
tags:
- oauth2
- expiration
---
---

^ 同事有问到Oauth2 token过期时间多少，查看了一下资料和源码，整理如下。

应用(client)向Oauth2获取token的时候，会根据各个应用的配置（ClientDetail）设置token的过期时间。


参考BaseClientDetails中的字段定义：

```
public class BaseClientDetails implements ClientDetails {

	@org.codehaus.jackson.annotate.JsonProperty("access_token_validity")
	@com.fasterxml.jackson.annotation.JsonProperty("access_token_validity")
	private Integer accessTokenValiditySeconds;

	@org.codehaus.jackson.annotate.JsonProperty("refresh_token_validity")
	@com.fasterxml.jackson.annotation.JsonProperty("refresh_token_validity")
	private Integer refreshTokenValiditySeconds;
}
```
对应db中OAUTH_CLIENT_DETAIL表中的access_token_validity和refresh_token_validity字段。
分别表示token和refresh token的过期时间。这两个字段在创建token的时候会被使用到，参考DefaultTokenServices中的以下方法。


```
public class DefaultTokenServices implements AuthorizationServerTokenServices, ResourceServerTokenServices,
		ConsumerTokenServices, InitializingBean {

	private int refreshTokenValiditySeconds = 60 * 60 * 24 * 30; // default 30 days.

	private int accessTokenValiditySeconds = 60 * 60 * 12; // default 12 hours.

	private OAuth2RefreshToken createRefreshToken(OAuth2Authentication authentication) {
		if (!isSupportRefreshToken(authentication.getOAuth2Request())) {
			return null;
		}
		int validitySeconds = getRefreshTokenValiditySeconds(authentication.getOAuth2Request());
		String value = UUID.randomUUID().toString();
		if (validitySeconds > 0) {
			return new DefaultExpiringOAuth2RefreshToken(value, new Date(System.currentTimeMillis()
					+ (validitySeconds * 1000L)));
		}
		return new DefaultOAuth2RefreshToken(value);
	}

	private OAuth2AccessToken createAccessToken(OAuth2Authentication authentication, OAuth2RefreshToken refreshToken) {
		DefaultOAuth2AccessToken token = new DefaultOAuth2AccessToken(UUID.randomUUID().toString());
		int validitySeconds = getAccessTokenValiditySeconds(authentication.getOAuth2Request());
		if (validitySeconds > 0) {
			token.setExpiration(new Date(System.currentTimeMillis() + (validitySeconds * 1000L)));
		}
		token.setRefreshToken(refreshToken);
		token.setScope(authentication.getOAuth2Request().getScope());

		return accessTokenEnhancer != null ? accessTokenEnhancer.enhance(token, authentication) : token;
	}

	/**
	 * The access token validity period in seconds
	 *
	 * @param clientAuth the current authorization request
	 * @return the access token validity period in seconds
	 */
	protected int getAccessTokenValiditySeconds(OAuth2Request clientAuth) {
		if (clientDetailsService != null) {
			ClientDetails client = clientDetailsService.loadClientByClientId(clientAuth.getClientId());
			Integer validity = client.getAccessTokenValiditySeconds();
			if (validity != null) {
				return validity;
			}
		}
		return accessTokenValiditySeconds;
	}

	/**
	 * The refresh token validity period in seconds
	 *
	 * @param clientAuth the current authorization request
	 * @return the refresh token validity period in seconds
	 */
	protected int getRefreshTokenValiditySeconds(OAuth2Request clientAuth) {
		if (clientDetailsService != null) {
			ClientDetails client = clientDetailsService.loadClientByClientId(clientAuth.getClientId());
			Integer validity = client.getRefreshTokenValiditySeconds();
			if (validity != null) {
				return validity;
			}
		}
		return refreshTokenValiditySeconds;
	}

}
```
access_token_validity和refresh_token_validity均按照秒数进行配置，过期时间为当前时间加速配置秒数值。

- 如果 access_token_validity 为空，则access token 过期时间默认为12小时；
- 如果 access_token_validity 为0，则access token 永远不会过期；
- 如果 refresh_token_validity 为空，则refresh token 过期时间为30天；
- 如果 refresh_token_validity 为0，则refresh token 永远不会过期；

{{ page.date | date_to_string }},{{ page.author }}

