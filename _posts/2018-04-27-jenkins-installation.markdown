---
author: 望哥
comments: true
date: 2018-04-27
title: jenkins installation on centos
categories:
- expirence
tags:
- jenkins

---


# 1. Preparation

## 1.1 Install JDK

```
yum install java-1.8.0-openjdk
```


# 2. Install Jenkins

## 2.1 Install from yum

```
sudo wget -O /etc/yum.repos.d/jenkins.repo https://pkg.jenkins.io/redhat/jenkins.repo
sudo rpm --import https://pkg.jenkins.io/redhat/jenkins.io.key
yum install jenkins
```

## 2.2 Config Jenkins Service

modify file: `/etc/sysconfig/jenkins`

```
JENKINS_PORT="7070"
```

start jenkins service:
```
sudo systemctl start jenkins.service
sudo systemctl status jenkins.service
```

view jenkins: `http://<ip>:7070`


## 2.3 Config Jenkins Plugins

First, to get admin password `cat /var/lib/jenkins/secrets/initialAdminPassword` .

Visit `http://<ip>:7070`, enter admin password to unlock jenkins.

Create a admin user to manage jenkins config:
```
name: admin
password: password
full name: admin
email: admin@example.com
```

Then, select plugins you needed to install, the following is what i select:

- Folders
- OWASP Markup Formatter
- Build Timeout
- Credentials Binding
- Timestamper
- Workspace Cleanup
- Ant
- Gradle
- Pipeline
- GitHub Branch Source
- Pipeline: GitHub Groovy Libraries
- Pipeline: Stage View
- Git
- SSH Slaves
- Matrix Authorization Strategy
- PAM Authentication
- Email Extension
- Mailer
- Dashboard View
- Config File Provider
- embeddable-build-status
- Rebuilder
- SSH Agent
- build-name-setter
- Throttle Concurrent Builds
- NodeJS
- Checkstyle
- Cobertura
- HTML Publisher
- JUnit
- Warnings
- Build Pipeline
- Conditional BuildStep
- Multijob
- Parameterized Trigger
- Copy Artifact
- Git Parameter
- GitHub
- GitLab
- Role-based Authorization Strategy
- Email Extension Template


Other plugins can be installed from page `Jenkins > Plugin Manager > Available`:
- pipeline-utility-steps
- Pipeline Maven Integration
- JIRA
- FindBugs
- Publish Over SSH
- SSH
- Dingding[钉钉]



## 2.4 Config Jenkins Security

Go to page `Jenkins > Configure Global Security` to config jenkins security as follow:

```
Configure Global Security
  Enable security
    Jenkins’ own user database
      Allow users to sign up
    Matrix-based security
      Anonymous Users: <NO PERMSSION SELECTED>
      Authenticated Users: <ALL PERSSIONS EXCEPT ADMINISTER>
      admin(user): <ONLY ADMINISTER PERMISSION>
```


create some users for team members so that they can create jenkins project.


## 2.5 How to deploy a custom plugin

1. Stop Jenkins
2. Copy the custom HPI file to $JENKINS_HOME/plugins
3. Remove the previously expanded plugin directory
4. Create an empty file called <plugin>.hpi.pinned - e.g. maven-plugin.hpi.pinned
5. Start Jenkins


# 3. Trouble Shooting:
## 3.1 Reset Jenkins Permission:

When configing jenkins security incorrectly, you may lose the control of jenkins.
You can reset jenkins permission as following and config security again.

1. sudo systemctl stop jenkins
2. rm -rf /var/lib/jenkins/users/*
3. vi /var/lib/jenkins/config.xml

```
<useSecurity>false</useSecurity>
# AND delete authorizationStrategy、securityRealm
```

4. sudo systemctl start jenkins
5. config jenkins enable secrity, and assign all permission to authrized user
6. go to login page to create one user

{{ page.date | date_to_string }},{{ page.author }}