# minitalk
**Bu projenin amacı *UNIX sinyallerini* kullanarak küçük bir veri değişim programı kodlamaktır .**
**Süreç Yönetimi:**

- **`getpid`**: Çağıran sürecin süreç kimliğini (process ID - PID) döndürür. Her sürecin benzersiz bir PID'si vardır. Örneğin: `pid_t pid = getpid();` geçerli sürecin PID'sini `pid` değişkenine atar.

## pid_t nedir?

`pid_t`  Unix ve Linux'ta muhtemelen işaretsiz bir int olan süreç tanımlayıcısının türüdür. Bir ps (süreç durumu) komutunun ilk sütununda görünen sayıdır:
`1. $ ps 
2.       PID    PPID    PGID       TTY         UID    STIME COMMAND 
3.      9252     772    9252      pty0      197609 14:02:00 /usr/bin/ps 
4. S    8984     772    8984      pty0      197609   Oct 22 /usr/bin/vi 
5.       772    9232     772      pty0      197609   Oct 22 /usr/bin/bash 
6.      9232       1    9232      ?         197609   Oct 22 /usr/bin/mintty`

İkinci sütun ebeveynin işlem kimliğidir. Zinciri görebiliriz: mintty sistem tarafından başlatıldı, burada Cygwin, işlem kimliği 1. Mintty, hem vi hem de ps'yi başlatan bash işlemini başlattı.

Yeni süreçler sistem çağrısı veya onun varyantlarından biriyle oluşturulur `pid=fork()`. Fork aslında çalışan bir süreci kopyalar. Ana süreç olarak adlandırılan orijinal sürece fork, asla sıfır olmayan süreç kimliğini döndürür. Yeni süreç olan çocuğa fork, 0 döndürür.

*Hap Bilgi :*

> *ppıd ne zaman aynı olur?*
> 

`*ppid` (Parent Process ID), bir sürecin ebeveyninin (parent process) kimliğini temsil eder.*

**`*ppid` ne zaman aynı olur?***

- ***Aynı ebeveyn tarafından oluşturulan süreçler**: Eğer iki süreç aynı ebeveyn tarafından oluşturulmuşsa, bu süreçlerin `ppid`'leri aynı olacaktır. Örneğin, bir programda `fork()` çağrısıyla birden fazla çocuk süreci oluşturulursa, hepsinin `ppid` değeri aynı olur, çünkü aynı ebeveynden (parent) türemişlerdir.*
- ***Orphan süreçler**: Eğer bir süreç, ebeveyn süreci sonlanırsa (terminate), bu süreç **yetim süreç (orphan process)** haline gelir ve sistem tarafından `init` sürecine (Linux'ta `pid=1`) atanır. Bu durumda, bu süreçlerin hepsi için `ppid=1` olur.*

Ebeveyn (veya uygun kimlik bilgilerine sahip, işlem kimliğine sahip herhangi bir işlem), bunu çocuğu kontrol etmek için kullanabilir. Genellikle ebeveyn sadece yapar `wait(…)`veya `waitid(pid, …)`çocuk bitirene kadar orada oturur. Çocuğu kullanarak bir sinyal gönderebilir `kill(pid, signum)`veya bir hata ayıklayıcı gibi davranabilir ve çocuğun durumunu kullanarak inceleyebilir `ptrace(pid, …)`, işlem kimliği için birkaç kullanımdan bahsetmek gerekirse.

**Sinyal İşleme:**

Sinyaller, işletim sisteminin süreçlere gönderdiği bildirimlerdir. Örneğin, Ctrl+C tuşlarına basmak bir `SIGINT` sinyali gönderir.

- **`signal` (Eski Yöntem)**: Bir sinyal için bir sinyal işleyici (signal handler) atamak için kullanılan eski bir yöntemdir. Ancak, bazı dezavantajları nedeniyle (özellikle güvenilirlik ve taşınabilirlik açısından) `sigaction` tercih edilir.
- **`sigemptyset` & `sigaddset`**:
    - **`sigemptyset`**: Bir sinyal kümesini başlatmak için kullanılır. Kümedeki tüm sinyalleri temizler.
    - **`sigaddset`**: Bir sinyali bir sinyal kümesine eklemek için kullanılır. Bu fonksiyonlar genellikle `sigaction` ile birlikte kullanılır.
- **`sigaction`**: Sinyal işleme için daha gelişmiş ve güvenilir bir arayüz sağlar. Bir sinyal için bir işleyici atamanın yanı sıra, sinyal maskelerini ve diğer davranışları da kontrol etmenizi sağlar.
- **`pause`**: Bir sinyal gelene kadar süreci askıya alır. Bir sinyal alındığında, `pause` fonksiyonu geri döner.
- **`kill`**: Bir sürece bir sinyal göndermek için kullanılır. Örneğin: `kill(pid, SIGTERM);` belirtilen PID'ye sahip sürece `SIGTERM` sinyali gönderir (genellikle süreci sonlandırmak için kullanılır).

**Zaman Yönetimi:**

- **`sleep`**: Belirtilen sayıda saniye boyunca süreci askıya alır.
- **`usleep`**: `sleep` fonksiyonuna benzer, ancak mikro saniye cinsinden askıya alma sağlar.

**Çıkış:**

- **`exit`**: Bir süreci sonlandırmak için kullanılır. Bir çıkış kodu alır. 0 genellikle başarılı bir çıkışı, 0'dan farklı bir değer ise bir hatayı gösterir.

*Daha fazla bilgi için -KESİNLİKELE!!- bunu oku: https://www.codequoi.com/en/creating-and-killing-child-processes-in-c/*

---

## SİNYALLER

`signal()` fonksiyonu, bir C programında **sinyal işleme** (signal handling) mekanizmasını kurmak için kullanılan bir fonksiyondur. Sinyaller, işletim sistemi tarafından bir sürece belirli olayları bildirmek için kullanılan bir yöntemdir. Örneğin, bir kullanıcı `Ctrl+C` tuşlarına bastığında, çalışmakta olan programa `SIGINT` sinyali gönderilir.

---

### **`signal()` Fonksiyonunun Prototipi:**

```c
c
KopyalaDüzenle
#include <signal.h>void (*signal(int signum, void (*handler)(int)))(int);

```

- **`signum`**: İşlem sırasında yakalanması istenen sinyalin türü. Örneğin:
    - `SIGINT` (klavyeden kesinti, genellikle `Ctrl+C` ile gönderilir),
    - `SIGTERM` (programı düzgün bir şekilde sonlandırmak için),
    - `SIGKILL` (programı anında sonlandırmak için, yakalanamaz).
- **`handler`**: Sinyal yakalandığında çağrılacak işlevin adresi.
    - `SIG_IGN`: Sinyali **yoksaymak** için kullanılır.
    - `SIG_DFL`: Sinyali varsayılan davranışla **işlemek** için kullanılır.
    - Kullanıcı tanımlı bir işlev adresi: Sinyal yakalandığında bu işlev çalıştırılır.
- Dönüş değeri: Önceki sinyal işleyicinin adresini döndürür.

**Sinyallerin Temel Amacı:**

Sinyallerin temel amacı, bir sürece belirli bir olayın meydana geldiğini bildirmektir. Bu olaylar şunlar olabilir:

- **Kullanıcı Etkileşimi:** Kullanıcının klavyeden bir tuşa basması (örneğin, Ctrl+C), bir fare tıklaması veya terminalden bir komut vermesi.
- **Hata Durumları:** Bir programda bir hata oluşması (örneğin, sıfıra bölme, geçersiz bellek erişimi).
- **Süreç Yönetimi:** Başka bir sürecin bir süreci sonlandırması veya bir alt sürecin durumunun değişmesi.
- **Zamanlayıcılar:** Belirli bir süre geçtikten sonra bir olayın tetiklenmesi.

**Sinyallerin Çalışma Mekanizması:**

1. **Olayın Meydana Gelmesi:** Bir olay gerçekleştiğinde (örneğin, kullanıcı Ctrl+C'ye bastığında), işletim sistemi ilgili sürece bir sinyal gönderir.
2. **Sinyalin Gönderilmesi:** İşletim sistemi, hedef sürecin süreç kontrol bloğuna (PCB) sinyal bilgisini ekler.
3. **Sürecin Kontrolü:** Süreç bir sonraki uygun zamanda (genellikle bir sistem çağrısından dönüşte veya bir kesme işlendikten sonra) bekleyen sinyalleri kontrol eder.
4. **Sinyal İşleyicisi (Signal Handler):** Eğer süreç, gelen sinyal için bir sinyal işleyicisi tanımlamışsa, bu işleyici fonksiyonu çalıştırılır. Sinyal işleyicisi, sinyale özgü bir eylemi gerçekleştirmek için tasarlanmış bir fonksiyondur.
5. **Varsayılan Davranış (Default Action):** Eğer süreç sinyal için bir işleyici tanımlamamışsa, işletim sisteminin o sinyal için tanımladığı varsayılan davranış uygulanır. Varsayılan davranışlar şunlar olabilir:
    - **Süreci Sonlandırma (Terminate):** Örneğin, `SIGSEGV` (geçersiz bellek erişimi) sinyalinin varsayılan davranışı süreci sonlandırmaktır.
    - **Süreci Durdurma (Stop):** Örneğin, `SIGSTOP` sinyali süreci durdurur.
    - **Süreci Devam Ettirme (Continue):** Örneğin, `SIGCONT` sinyali durdurulmuş bir süreci devam ettirir.
    - **Sinyali Yok Sayma (Ignore):** Bazı sinyaller varsayılan olarak yok sayılır.

**Örnekler ve Açıklamalar:**

- **Ctrl+C (`SIGINT`):** Kullanıcı terminalde çalışan bir süreci durdurmak için Ctrl+C tuşlarına bastığında, terminal sürücüsü çalışan sürece `SIGINT` sinyali gönderir. Genellikle bu sinyalin varsayılan davranışı süreci sonlandırmaktır. Ancak, süreç bu sinyal için bir işleyici tanımlayarak farklı bir eylem gerçekleştirebilir (örneğin, temizleme işlemleri yapıp sonra sonlanmak).
- **`kill` Komutu:** `kill` komutu, bir sürece belirli bir sinyal göndermek için kullanılır. Örneğin, `kill -9 <PID>` komutu, belirtilen PID'ye sahip sürece `SIGKILL` sinyali gönderir. `SIGKILL`, yakalanamayan ve bloke edilemeyen bir sinyaldir; bu nedenle süreci hemen sonlandırır.
- **Sıfıra Bölme (`SIGFPE`):** Bir programda sıfıra bölme gibi bir aritmetik hata oluştuğunda, işletim sistemi sürece `SIGFPE` sinyali gönderir. Bu sinyalin varsayılan davranışı genellikle süreci sonlandırmaktır.
- **Geçersiz Bellek Erişimi (`SIGSEGV`):** Bir program, kendisine ait olmayan bir bellek alanına erişmeye çalıştığında, işletim sistemi sürece `SIGSEGV` sinyali gönderir. Bu sinyalin varsayılan davranışı da süreci sonlandırmaktır.

**Özetle:**

Sinyaller, işletim sistemi ve süreçler arasında ve süreçlerin kendi aralarında asenkron iletişim kurmasını sağlayan güçlü bir mekanizmadır. Olaylara hızlı bir şekilde tepki vermeyi, hataları işlemeyi ve süreçleri yönetmeyi mümkün kılarlar. `signal.h` başlık dosyası ve ilgili sistem çağrıları (`signal`, `sigaction`, `kill` vb.) bu mekanizmayı programlarımızda kullanmamızı sağlar.

![mini](https://github.com/user-attachments/assets/93b76cc0-d2f8-4dda-86a0-c0f94bb9b27a)
Kaynak: https://42-cursus.gitbook.io/guide/rank-02/minitalk/understand-minitalk

Bu görsel, bir **client** (istemci) ve **server** (sunucu) arasında sinyallerle veri iletiminin nasıl gerçekleştiğini açıklıyor.

---

### **Client ve Server Arasında "Hello" Gönderimi**

1. **İletişim Başlangıcı**:
    - İstemci, `"Hello"` kelimesini server'a göndermek istiyor. Ancak `"Hello"` gibi bir string (metin) doğrudan sinyallerle gönderilemez.
    

---

1. **ASCII Dönüşümü**:
    - `"Hello"` stringindeki her karakter, ASCII tablosuna göre bir sayıya dönüştürülür:
        - `H -> 72`
        - `e -> 101`
        - `l -> 108`
        - `l -> 108`
        - `o -> 111`

---

1. **Binary (İkili) Dönüşüm**:
    - Bu ASCII değerleri, binary (ikili) formata çevrilir:
        - `H (72)` → `01001000`
        - `e (101)` → `01100101`
        - `l (108)` → `01101100`
        - `l (108)` → `01101100`
        - `o (111)` → `01101111`
    - Her karakterin ASCII değeri 8-bitlik binary veriye çevrildiği için toplamda:
        - 5 harf x 8 bit = **40 bit** sinyal iletilir.

---

1. **Sinyallerin Gönderimi**:
    - Bu binary veriler, **bit bit** (0 ve 1'ler şeklinde) sinyallerle server'a gönderilir.
    - Her 0 ve 1 bir sinyal dalgasını temsil eder:
        - Örneğin: `0, 1, 0, 0, 1, 0, 0, 0` şeklinde ilerler.
    - İstemci bu sinyalleri gönderirken, her bit birer birer server'a iletilir.

---

1. **Server Tarafında Alınan Veri**:
    - Server, gelen sinyalleri toplar ve binary veriye dönüştürür.
    - Binary veri tekrar ASCII'ye çevrilir:
        - `01001000` → `H`
        - `01100101` → `e`
        - `01101100` → `l`
        - `01101100` → `l`
        - `01101111` → `o`
    - Bu işlem sonucunda server, `"Hello"` stringini yeniden oluşturur.

---




