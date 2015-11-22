---
author: admin
comments: true
date: 2013-04-07 03:58:35+00:00
excerpt: Change JDBC Positional parameter style to JPA-style positional parameters
  style for Hibernate HQL
layout: post
slug: change-jdbc-positional-parameter-style-to-jpa-style-positional-parameters-style-for-hibernate-hql
title: Change JDBC Positional parameter style to JPA-style positional parameters style
  for Hibernate HQL
wordpress_id: 424
categories:
- Experience
tags:
- hibernate
- hql
- parameter
---

After update hibernate from version 3 to version 4, the old HQL will throw the following error.

       org.hibernate.hql.internal.ast.HqlSqlWalker [HqlSqlWalker.java:929] [DEPRECATION] Encountered positional parameter near line 1, column 56. Positional parameter are considered deprecated; use named parameters or JPA-style positional parameters instead.


In the projects, HQLs are most often generated dynamicly.

    StringBuffer hql = new StringBuffer("from Record where 1=1");
    	List params = new ArrayList();
    	if (subject!= null && subject.trim().length() > 0) {
    		hql.append(" and subject like ?");
    		params.add("%" + subject.trim() + "%");
    	}
    	if (startDate != null) {
    		hql.append(" and createTime> ?");
    		params.add(startDate);
    	}
    	if (endDate != null) {
    		hql.append(" and createTime< ?");
    		params.add(endDate);
    	}	
    	List result = queryByHQL(hql,params);
    	//common query method
    	List queryByHQL(String hql,List params){
    		Query query = getSession().createQuery(hql);
    		if (params.length > 0) {
    		   for (int i = 0, length = params.length; i < length; i++)
    				query.setParameter(i, params.get(i));
    		}
    		return query.list(); 
    	}


If changed to named parameter style , the code above would be like:

    StringBuffer hql = new StringBuffer("from Record where 1=1");
    	Map params = new HashMap();
    	if (subject!= null && subject.trim().length() > 0) {
    		hql.append(" and subject like :subject");
    		params.put("subject","%" + subject.trim() + "%");
    	}
    	if (startDate != null) {
    		hql.append(" and createTime> :startDate");
    		params.put("startDate",startDate);
    	}
    	if (endDate != null) {
    		hql.append(" and createTime< :endDate");
    		params.put("endDate",endDate);
    	}	
    	List result = queryByHQL(hql,params);
    	//common query method
    	List queryByHQL(String hql,Map params){
    		Query query = getSession().createQuery(hql);
    		if (params!=null) {
    			Set keySet = params.keySet();
    			for (String key : keySet) {
    				query.setParameter(key, params.get(key));
    			}
    		}
    		return query.list(); 
    	}
    

The named parameters seem redundant.If changed to JPA-style positional parameters style , the code would be like:

    StringBuffer hql = new StringBuffer("from Record where 1=1");
    	Map params = new HashMap();
    	if (subject!= null && subject.trim().length() > 0) {
    		hql.append(" and subject like ?0");
    		params.put("0","%" + subject.trim() + "%");
    	}
    	if (startDate != null) {
    		hql.append(" and createTime> ?1");
    		params.put("1",startDate);
    	}
    	if (endDate != null) {
    		hql.append(" and createTime< ?2");
    		params.put("2",endDate);
    	}
    	List result = queryByHQL(hql,params);
    	//common query method
    	List queryByHQL(String hql,Map params){
    		Query query = getSession().createQuery(hql);
    		if (params!=null) {
    			Set keySet = params.keySet();
    			for (String key : keySet) {
    				query.setParameter(key, params.get(key));
    			}
    		}
    		return query.list(); 
    	}


OH!The JPA-style positional parameters style just add a order number after every '?' . It's easy to change JDBC Positional parameter style to JPA-style positional parameters style.


    //common query method
    	List queryByHQL(String hql,List params){
    		Query query = getSession().createQuery(format(hql));
    		setParameterList(query,params);
    		return query.list(); 
    	}
    	//fomat HQL to JPQL style
    	private String fomatHQL(CharSequence queryString) {     
    		StringBuffer buffer = new StringBuffer(queryString);
    		int start = 0;
    		int order = 0;
    		while ((start = buffer.indexOf("?", start + 1)) > 0) {
    			buffer.insert(start + 1, order);
    			order++;
    		}
    		return buffer.toString();
        }
    	protected void setParameterList(Query query, Object[] params) {
            if (params != null && params.length > 0) {
                for (int i = 0, length = params.length; i < length; i++)
                    query.setParameter(i + "", params[i]);
            }
        }


Then, we can still use the JDBC Positional parameter style HQLs without warning.

Reference:
1. Deprecate HQL-specific (JDBC-style) positional parameters, [https://hibernate.atlassian.net/browse/HHH-7023](https://hibernate.atlassian.net/browse/HHH-7023)
