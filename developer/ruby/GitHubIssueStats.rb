require 'github_api'
require 'date'
require 'yaml'

begin_date = Time.parse('2017-09-06' + 'T06:00:00Z')

end_date = Time.now
end_date = Time.parse('2017-09-13' + 'T006:00:00Z')

repo_owner = 'NREL'
repo = 'OpenStudio'

github = Github.new
if File.exists?(Dir.home + '/github_config.yml')
  github_options = YAML.load_file(Dir.home + '/github_config.yml')
  token = github_options['oauth_token']
  github = Github.new oauth_token: token
end

totalOpenIssues = Array.new
totalOpenPullRequests = Array.new
newIssues = Array.new
newOpenIssues = Array.new
closedIssues = Array.new
acceptedPullRequests = Array.new

def get_num(issue)
  issue.html_url.split('/')[-1].to_i
end

def get_issue_num(issue)
  "\##{get_num(issue)}"
end

def get_html_url(issue)
  issue.html_url
end

def get_title(issue)
  issue.title
end

def print_issue(issue)
  is_feature = false
  issue.labels.each {|label| is_feature = true if label.name == "Feature Request"}
  
  if is_feature
    "- ![Improved:][improved] [#{get_issue_num(issue)}]( #{get_html_url(issue)} ), #{get_title(issue)}"
  else
    "- ![Fixed:][fixed] [#{get_issue_num(issue)}]( #{get_html_url(issue)} ), #{get_title(issue)}"
  end
end

# Process Open Issues
results = -1
page = 1
while (results != 0)
  resp = github.issues.list user: repo_owner, repo: repo,
                            :sort => 'created',
                            :direction => 'asc',
                            :state => 'open',
                            :per_page => 100,
                            :page => page
  results = resp.length
  resp.env[:body].each do |issue, index|
    created = Time.parse(issue.created_at)
    if !issue.has_key?(:pull_request)
      totalOpenIssues << issue
      if created >= begin_date && created <= end_date
        newIssues << issue
      end
    else
      totalOpenPullRequests << issue
    end
  end

  page = page + 1
end

# Process Closed Issues
results = -1
page = 1
while (results != 0)
  resp = github.issues.list user: repo_owner, repo: repo,
                            :sort => 'created',
                            :direction => 'asc',
                            :state => 'closed',
                            :per_page => 100,
                            :page => page
  results = resp.length
  resp.env[:body].each do |issue, index|
    created = Time.parse(issue.created_at)
    closed = Time.parse(issue.closed_at)
    if !issue.has_key?(:pull_request)
      if created >= begin_date && created <= end_date
        newIssues << issue
      end
      if closed >= begin_date && closed <= end_date
        closedIssues << issue
      end
    elsif closed >= begin_date && closed <= end_date
      acceptedPullRequests << issue
    end
  end

  page = page + 1
end

closedIssues.sort! {|x,y| get_num(x) <=> get_num(y)}
newIssues.sort! {|x,y| get_num(x) <=> get_num(y)}
acceptedPullRequests.sort! {|x,y| get_num(x) <=> get_num(y)}
totalOpenPullRequests.sort! {|x,y| get_num(x) <=> get_num(y)}

puts "Total Open Issues: #{totalOpenIssues.length}"
puts "Total Open Pull Requests: #{totalOpenPullRequests.length}"
puts "\nDate Range: #{begin_date.strftime('%m/%d/%y')} - #{end_date.strftime('%m/%d/%y')}:"
puts "\nNew Issues: #{newIssues.length} (" + newIssues.map{|issue| get_issue_num(issue)}.join(', ') + ')'

puts "\nClosed Issues: #{closedIssues.length} (" + closedIssues.map{|issue| get_issue_num(issue)}.join(', ') + ')'
closedIssues.each{|issue| puts print_issue(issue)}

puts "\nAccepted Pull Requests: #{acceptedPullRequests.length} (" + acceptedPullRequests.map{|issue| get_issue_num(issue)}.join(', ') + ')'
acceptedPullRequests.each{|issue| puts print_issue(issue)}

puts "\nAll Open Issues: #{totalOpenIssues.length} (" + totalOpenIssues.map{|issue| get_issue_num(issue)}.join(', ') + ')'
